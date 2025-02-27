#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "kmp.h"

namespace esphome {
namespace multical402 {

static const char *const TAG = "multical402";

class Multical402Component : public PollingComponent, public uart::UARTDevice {
 public:
  Multical402Component() = default;
  
  // Implementation moved from multical402.cpp
  void setup() override {
    ESP_LOGCONFIG(TAG, "Setting up Multical402...");
    kmp_.setup(this);
  }

  void update() override {
    ESP_LOGD(TAG, "Reading meter values...");
    
    if (this->energy_sensor_ != nullptr) {
      float value = kmp_.get_energy();
      ESP_LOGD(TAG, "Energy: %.3f", value);
      this->energy_sensor_->publish_state(value);
    }
    
    if (this->power_sensor_ != nullptr) {
      float value = kmp_.get_power();
      ESP_LOGD(TAG, "Power: %.1f", value);
      this->power_sensor_->publish_state(value);
    }
    
    if (this->temp_in_sensor_ != nullptr) {
      float value = kmp_.get_temp_in();
      ESP_LOGD(TAG, "Temp in: %.2f", value);
      this->temp_in_sensor_->publish_state(value);
    }
    
    if (this->temp_out_sensor_ != nullptr) {
      float value = kmp_.get_temp_out();
      ESP_LOGD(TAG, "Temp out: %.2f", value);
      this->temp_out_sensor_->publish_state(value);
    }
    
    if (this->temp_diff_sensor_ != nullptr) {
      float value = kmp_.get_temp_diff();
      ESP_LOGD(TAG, "Temp diff: %.2f", value);
      this->temp_diff_sensor_->publish_state(value);
    }
    
    if (this->flow_sensor_ != nullptr) {
      float value = kmp_.get_flow();
      ESP_LOGD(TAG, "Flow: %.0f", value);
      this->flow_sensor_->publish_state(value);
    }
    
    if (this->volume_sensor_ != nullptr) {
      float value = kmp_.get_volume();
      ESP_LOGD(TAG, "Volume: %.2f", value);
      this->volume_sensor_->publish_state(value);
    }
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "Multical402:");
    LOG_UPDATE_INTERVAL(this);
    LOG_SENSOR("  ", "Energy", this->energy_sensor_);
    LOG_SENSOR("  ", "Volume", this->volume_sensor_);
    LOG_SENSOR("  ", "Temperature In", this->temp_in_sensor_);
    LOG_SENSOR("  ", "Temperature Out", this->temp_out_sensor_);
    LOG_SENSOR("  ", "Temperature Difference", this->temp_diff_sensor_);
    LOG_SENSOR("  ", "Power", this->power_sensor_);
    LOG_SENSOR("  ", "Flow", this->flow_sensor_);
  }
  
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