#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace multical402 {

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
  
 protected:
  uart::UARTDevice *_uart{nullptr};
  
  // kamReadReg - read a Kamstrup register
  float Read(unsigned int registerId);
  
  // kamSend - send data to Kamstrup meter
  void Send(char const *msg, int msgsize);
  
  // kamReceive - receive bytes from Kamstrup meter
  unsigned short Receive(char recvmsg[]);
  
  // kamDecode - decodes received data
  float Decode(const unsigned int registerId, const char *msg);
  
  // crc_1021 - calculate crc16
  long crc_1021(char const *inmsg, unsigned int len);
};

}  // namespace multical402
}  // namespace esphome