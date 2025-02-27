import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    CONF_ID,
    CONF_UPDATE_INTERVAL,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_CELSIUS,
    UNIT_CUBIC_METER,
    UNIT_KILOWATT
)

# Define custom unit strings for the ones not available
UNIT_LITER_PER_HOUR = "L/h"
UNIT_MEGAWATT_HOUR = "MWh"

# Define namespaces and component names
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

multical402_ns = cg.esphome_ns.namespace("multical402")
Multical402Component = multical402_ns.class_(
    "Multical402Component", cg.PollingComponent, uart.UARTDevice
)

# Configuration options
CONF_ENERGY = "energy"
CONF_POWER = "power"
CONF_TEMP_IN = "temperature_in"
CONF_TEMP_OUT = "temperature_out"
CONF_TEMP_DIFF = "temperature_diff"
CONF_FLOW = "flow"
CONF_VOLUME = "volume"

# Schema definition
CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(Multical402Component),
        cv.Optional(CONF_UPDATE_INTERVAL, default="60s"): cv.update_interval,
        cv.Optional(CONF_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_MEGAWATT_HOUR,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_VOLUME): sensor.sensor_schema(
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMP_IN): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMP_OUT): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMP_DIFF): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FLOW): sensor.sensor_schema(
            unit_of_measurement=UNIT_LITER_PER_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    })
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_ENERGY in config:
        sens = await sensor.new_sensor(config[CONF_ENERGY])
        cg.add(var.set_energy_sensor(sens))
    
    if CONF_VOLUME in config:
        sens = await sensor.new_sensor(config[CONF_VOLUME])
        cg.add(var.set_volume_sensor(sens))
    
    if CONF_TEMP_IN in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_IN])
        cg.add(var.set_temp_in_sensor(sens))
    
    if CONF_TEMP_OUT in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_OUT])
        cg.add(var.set_temp_out_sensor(sens))
    
    if CONF_TEMP_DIFF in config:
        sens = await sensor.new_sensor(config[CONF_TEMP_DIFF])
        cg.add(var.set_temp_diff_sensor(sens))
    
    if CONF_POWER in config:
        sens = await sensor.new_sensor(config[CONF_POWER])
        cg.add(var.set_power_sensor(sens))
    
    if CONF_FLOW in config:
        sens = await sensor.new_sensor(config[CONF_FLOW])
        cg.add(var.set_flow_sensor(sens))