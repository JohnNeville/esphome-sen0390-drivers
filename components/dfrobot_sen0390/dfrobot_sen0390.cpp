#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "dfrobot_sen0390.h"

namespace esphome {
namespace dfrobot_sen0390 {

static const char *TAG = "dfrobot_sen0390.sensor";

void DFRobotSEN0390::setup() {
    ESP_LOGCONFIG(TAG, "Setting up DFRobot SEN0390...");
}

void DFRobotSEN0390::update() {
    uint8_t reg = 0x00;
    uint8_t data[4];

    // Read 4 bytes using the combined write/read method
    if (this->write_read(&reg, 1, data, 4) != i2c::ERROR_OK) {
        ESP_LOGW(TAG, "Communication failed: Could not read from SEN0390");
        this->status_set_warning();
        return;
    }

    this->status_clear_warning();

    // 1. Report Raw Data if configured
    if (this->raw_sensor_ != nullptr) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%02X %02X %02X %02X", data[0], data[1], data[2], data[3]);
        this->raw_sensor_->publish_state(buf);
    }

    // 2. Report Lux Data
    // Data is sent as 32-bit unsigned integer
    // Build value same way as DFRobot library: data[3] is MSB, data[0] is LSB
    // Formula from DFRobot library: (raw_value * 1.4) / 1000
    uint32_t raw_value = data[3];
    raw_value = (raw_value << 8) | data[2];
    raw_value = (raw_value << 8) | data[1];
    raw_value = (raw_value << 8) | data[0];
    float lux = (raw_value * 1.4f) / 1000.0f;

    if (this->lux_sensor_ != nullptr) {
        this->lux_sensor_->publish_state(lux);
    }

    // raw_value is uint32_t, which is long unsigned on this target — cast rather
    // than widen the format, so the log line stays portable.
    ESP_LOGD(TAG, "Raw: %02X%02X%02X%02X | Raw Value: %u | Lux: %.1f",
             data[0], data[1], data[2], data[3], static_cast<unsigned>(raw_value), lux);
}

void DFRobotSEN0390::dump_config() {
    ESP_LOGCONFIG(TAG, "DFRobot SEN0390 Custom Component");
    LOG_I2C_DEVICE(this);
}

}  // namespace dfrobot_sen0390
}  // namespace esphome
