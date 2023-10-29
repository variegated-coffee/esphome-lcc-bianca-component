import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_SECOND, ICON_RADIOACTIVE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE

from .. import open_lcc_bianca_ns, OpenLCCBianca

CONF_OPEN_LCC_BIANCA_ID = "open_lcc_bianca_id"

CONF_COFFEE_BOILER_TEMPERATURE = "coffee_boiler_temperature"
CONF_SERVICE_BOILER_TEMPERATURE = "service_boiler_temperature"
CONF_EXTERNAL_TEMPERATURE_1 = "external_temperature_1"
CONF_EXTERNAL_TEMPERATURE_2 = "external_temperature_2"
CONF_EXTERNAL_TEMPERATURE_3 = "external_temperature_3"
CONF_AUTO_SLEEP_IN = "auto_sleep_in"
CONF_RP2040_UPTIME = "rp2040_uptime"
CONF_BREW_TIME = "brew_time"

OpenLCCBiancaSensor = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaSensor",
    cg.Component,
    cg.Parented.template(OpenLCCBianca),
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OpenLCCBiancaSensor),
        cv.GenerateID(CONF_OPEN_LCC_BIANCA_ID): cv.use_id(OpenLCCBianca),
        cv.Optional(CONF_COFFEE_BOILER_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SERVICE_BOILER_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_AUTO_SLEEP_IN): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_RP2040_UPTIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EXTERNAL_TEMPERATURE_1): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EXTERNAL_TEMPERATURE_2): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EXTERNAL_TEMPERATURE_3): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BREW_TIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND,
            icon=ICON_RADIOACTIVE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_OPEN_LCC_BIANCA_ID])

    if coffee_boiler_temperature_config := config.get(CONF_COFFEE_BOILER_TEMPERATURE):
        sens = await sensor.new_sensor(coffee_boiler_temperature_config)
        cg.add(var.set_coffee_boiler_temperature(sens))
    if service_boiler_temperature_config := config.get(CONF_SERVICE_BOILER_TEMPERATURE):
        sens = await sensor.new_sensor(service_boiler_temperature_config)
        cg.add(var.set_service_boiler_temperature(sens))
    if auto_sleep_in_config := config.get(CONF_AUTO_SLEEP_IN):
        sens = await sensor.new_sensor(auto_sleep_in_config)
        cg.add(var.set_auto_sleep_in(sens))
    if rp2040_uptime_config := config.get(CONF_RP2040_UPTIME):
        sens = await sensor.new_sensor(rp2040_uptime_config)
        cg.add(var.set_rp2040_uptime(sens))
    if external_temp_1_config := config.get(CONF_EXTERNAL_TEMPERATURE_1):
        sens = await sensor.new_sensor(external_temp_1_config)
        cg.add(var.set_external_temperature_1(sens))
    if external_temp_2_config := config.get(CONF_EXTERNAL_TEMPERATURE_2):
        sens = await sensor.new_sensor(external_temp_2_config)
        cg.add(var.set_external_temperature_2(sens))
    if external_temp_3_config := config.get(CONF_EXTERNAL_TEMPERATURE_3):
        sens = await sensor.new_sensor(external_temp_3_config)
        cg.add(var.set_external_temperature_3(sens))
    if brew_time_config := config.get(CONF_BREW_TIME):
        sens = await sensor.new_sensor(brew_time_config)
        cg.add(var.set_brew_time(sens))