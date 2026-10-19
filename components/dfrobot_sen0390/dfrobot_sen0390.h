#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace dfrobot_sen0390 {

class DFRobotSEN0390 : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  void set_lux_sensor(sensor::Sensor *s) { lux_sensor_ = s; }
  void set_raw_sensor(text_sensor::TextSensor *s) { raw_sensor_ = s; }

 protected:
  sensor::Sensor *lux_sensor_{nullptr};
  text_sensor::TextSensor *raw_sensor_{nullptr};
};

}  // namespace dfrobot_sen0390
}  // namespace esphome
