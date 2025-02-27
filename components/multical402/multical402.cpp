#include "multical402.h"
#include "esphome/core/log.h"

namespace esphome {
namespace multical402 {

static const char *TAG = "multical402";

void Multical402Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Multical402...");
  kmp_.setup(this);
}

void Multical402Component::update() {
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

void Multical402Component::dump_config() {
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

}  // namespace multical402
}  // namespace esphome