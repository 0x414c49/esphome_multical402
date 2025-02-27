#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "kmp.h"

namespace esphome {
namespace multical402 {

class Multical402Component : public PollingComponent, public uart::UARTDevice {
 public:
  Multical402Component() = default;
  
  void setup() override;
  void update() override;
  void dump_config() override;
  
  void set_energy_sensor(sensor::Sensor *energy_sensor) { energy_sensor_ = energy_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_temp_in_sensor(sensor::Sensor *temp_in_sensor) { temp_in_sensor_ = temp_in_sensor; }
  void set_temp_out_sensor(sensor::Sensor *temp_out_sensor) { temp_out_sensor_ = temp_out_sensor; }
  void set_temp_diff_sensor(sensor::Sensor *temp_diff_sensor) { temp_diff_sensor_ = temp_diff_sensor; }
  void set_flow_sensor(sensor::Sensor *flow_sensor) { flow_sensor_ = flow_sensor; }
  void set_volume_sensor(sensor::Sensor *volume_sensor) { volume_sensor_ = volume_sensor; }
  
 protected:
  KMP kmp_{};
  
  sensor::Sensor *energy_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  sensor::Sensor *temp_in_sensor_{nullptr};
  sensor::Sensor *temp_out_sensor_{nullptr};
  sensor::Sensor *temp_diff_sensor_{nullptr};
  sensor::Sensor *flow_sensor_{nullptr};
  sensor::Sensor *volume_sensor_{nullptr};
};

}  // namespace multical402
}  // namespace esphome