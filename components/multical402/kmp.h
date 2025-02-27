#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include <math.h>

namespace esphome {
namespace multical402 {

static const char *const KMP_TAG = "multical402.kmp";

// Kamstrup timeout after transmit
const unsigned char TIMEOUT = 200;

enum DestinationAddress : unsigned char {
    HEAT_METER = 0x3f,
    LOGGER_TOP = 0x7f,
    LOGGER_BASE = 0xbf,
};

// Kamstrup Multical 402
// The registers we want to get out of the meter
const unsigned int registerIds[] = {
    0x003C, // 60 - Heat energy
    0x0050, // 80 - Current power
    0x0056, // 86 - Current forward temperature
    0x0057, // 87 - Current return temperature
    0x0059, // 89 - Current differential temperature
    0x004A, // 74 - Current water flow
    0x0044  // 68 - Volume register V1
};

class KMP {
 public:
  KMP() = default;
  
  void setup(uart::UARTDevice *parent) {
    _uart = parent;
  }

  float HeatEnergy() {
    return Read(registerIds[0]);
  }
  
  float CurrentPower() {
    return Read(registerIds[1]);
  }
  
  float CurrentForwardTemperature() {
    return Read(registerIds[2]);
  }
  
  float CurrentReturnTemperature() {
    return Read(registerIds[3]);
  }
  
  float CurrentDifferentialTemperature() {
    return Read(registerIds[4]);
  }
  
  float CurrentWaterFlow() {
    return Read(registerIds[5]);
  }
  
  float Volume() {
    return Read(registerIds[6]);
  }

  float get_energy() { return HeatEnergy(); }
  float get_power() { return CurrentPower(); }
  float get_temp_in() { return CurrentForwardTemperature(); }
  float get_temp_out() { return CurrentReturnTemperature(); }
  float get_temp_diff() { return CurrentDifferentialTemperature(); }
  float get_flow() { return CurrentWaterFlow(); }
  float get_volume() { return Volume(); }
  
  // Implementation of all methods - moved from kmp.cpp
  float Read(unsigned int registerId) {
    char recvmsg[40]; // buffer of bytes to hold the received data
    float rval = 0;   // this will hold the final value

    // prepare message to send and send it
    char sendmsg[] = {HEAT_METER, 0x10, 0x01,
                      static_cast<char>(registerId >> 8),
                      static_cast<char>(registerId & 0xff)};
    Send(sendmsg, 5);

    // listen if we get an answer
    unsigned short rxnum = Receive(recvmsg);

    // check if number of received bytes > 0
    if (rxnum != 0) {
        // decode the received message
        rval = Decode(registerId, recvmsg);
    }

    return rval;
  }
  
  void Send(char const *msg, int msgsize) {
    // append checksum bytes to message
    char newmsg[msgsize + 2];
    for (int i = 0; i < msgsize; i++) {
        newmsg[i] = msg[i];
    }
    newmsg[msgsize++] = 0x00;
    newmsg[msgsize++] = 0x00;
    int c = crc_1021(newmsg, msgsize);
    newmsg[msgsize - 2] = (c >> 8);
    newmsg[msgsize - 1] = c & 0xff;

    // build final transmit message - escape various bytes
    unsigned char txmsg[20] = {0x80}; // prefix
    unsigned int txsize = 1;
    for (int i = 0; i < msgsize; i++) {
        if (newmsg[i] == 0x06 || newmsg[i] == 0x0d || newmsg[i] == 0x1b || 
            newmsg[i] == 0x40 || newmsg[i] == 0x80) {
            txmsg[txsize++] = 0x1b;
            txmsg[txsize++] = newmsg[i] ^ 0xff;
        } else {
            txmsg[txsize++] = newmsg[i];
        }
    }
    txmsg[txsize++] = 0x0d; // EOL

    // send to serial interface
    _uart->write_array(txmsg, txsize);
  }
  
  unsigned short Receive(char recvmsg[]) {
    char rxdata[50]; // buffer to hold received data
    unsigned long rxindex = 0;
    unsigned long starttime = millis();

    _uart->flush(); // flush serial buffer - might contain noise

    char r = 0;

    // loop until EOL received or timeout
    while (r != 0x0d) {
        // handle rx timeout
        if (millis() - starttime > TIMEOUT) {
            ESP_LOGW(KMP_TAG, "Timed out listening for data");
            return 0;
        }

        // handle incoming data
        if (_uart->available()) {
            // receive byte
            r = _uart->read();
            if (r != 0x40) { // don't append if we see the start marker
                // append data
                rxdata[rxindex] = r;
                rxindex++;
            }
        }
    }

    // remove escape markers from received data
    unsigned short j = 0;
    for (unsigned short i = 0; i < rxindex - 1; i++) {
        if (rxdata[i] == 0x1b) {
            char v = rxdata[i + 1] ^ 0xff;
            if (v != 0x06 && v != 0x0d && v != 0x1b && v != 0x40 && v != 0x80) {
                ESP_LOGW(KMP_TAG, "Missing escape %X", v);
            }
            recvmsg[j] = v;
            i++; // skip
        } else {
            recvmsg[j] = rxdata[i];
        }
        j++;
    }

    // check CRC
    if (crc_1021(recvmsg, j)) {
        ESP_LOGW(KMP_TAG, "CRC error");
        return 0;
    }

    return j;
  }
  
  float Decode(const unsigned int registerId, const char *msg) {
    // skip if message is not valid
    if (msg[0] != 0x3f || msg[1] != 0x10) {
        ESP_LOGW(KMP_TAG, "Invalid message header");
        return 0;
    }
    
    if (msg[2] != (registerId >> 8) || msg[3] != (registerId & 0xff)) {
        ESP_LOGW(KMP_TAG, "Register ID mismatch");
        return 0;
    }

    // decode the mantissa
    long x = 0;
    for (int i = 0; i < msg[5]; i++) {
        x <<= 8;
        x |= msg[i + 7];
    }

    // decode the exponent
    int i = msg[6] & 0x3f;
    if (msg[6] & 0x40) {
        i = -i;
    }
    float ifl = pow(10, i);
    if (msg[6] & 0x80) {
        ifl = -ifl;
    }

    // return final value
    return (float)(x * ifl);
  }
  
  long crc_1021(char const *inmsg, unsigned int len) {
    long creg = 0x0000;
    for (unsigned int i = 0; i < len; i++) {
        int mask = 0x80;
        while (mask > 0) {
            creg <<= 1;
            if (inmsg[i] & mask) {
                creg |= 1;
            }
            mask >>= 1;
            if (creg & 0x10000) {
                creg &= 0xffff;
                creg ^= 0x1021;
            }
        }
    }
    return creg;
  }
  
 protected:
  uart::UARTDevice *_uart{nullptr};
};

}  // namespace multical402
}  // namespace esphome