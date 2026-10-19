import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor, text_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_ILLUMINANCE,
    ICON_BRIGHTNESS_5,
    STATE_CLASS_MEASUREMENT,
    UNIT_LUX,
)

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor", "text_sensor"]


dfrobot_sen0390_ns = cg.esphome_ns.namespace("dfrobot_sen0390")
DFRobotSEN0390 = dfrobot_sen0390_ns.class_(
    "DFRobotSEN0390", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DFRobotSEN0390),
            cv.Required("lux"): sensor.sensor_schema(
                unit_of_measurement=UNIT_LUX,
                icon=ICON_BRIGHTNESS_5,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_ILLUMINANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional("raw"): text_sensor.text_sensor_schema(),
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(i2c.i2c_device_schema(0x4A))
)


async def to_code(config):
    # Create the main controller object
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # NOTE: no cg.add_library("dfrobot/DFRobot_B_LUX_V30B") here on purpose.
    # This component talks to the sensor directly over i2c::I2CDevice and only
    # borrows the vendor library's lux formula (see dfrobot_sen0390.cpp), so the
    # dependency was unused. It is also Arduino-only — from ESPHome 2026.7 the
    # esp-idf build materialises PlatformIO libs as IDF managed components and
    # compiles them, so declaring it fails with "Arduino.h: No such file".

    # Register the Lux child sensor
    if "lux" in config:
        sens = await sensor.new_sensor(config["lux"])
        cg.add(var.set_lux_sensor(sens))

    # Register the Raw child text sensor
    if "raw" in config:
        tsens = await text_sensor.new_text_sensor(config["raw"])
        cg.add(var.set_raw_sensor(tsens))
