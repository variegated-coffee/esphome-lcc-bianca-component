import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_SECOND, ICON_RADIOACTIVE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE

from .. import open_lcc_bianca_ns, OpenLCCBianca

CONF_OPEN_LCC_BIANCA_ID = "open_lcc_bianca_id"

CONF_BREWING = "brewing"
CONF_FILLING_SERVICE_BOILER = "filling_service_boiler"
CONF_WATER_TANK_LOW = "water_tank_low"
CONF_BREW_BOILER_HEATING = "brew_boiler_heating"
CONF_SERVICE_BOILER_HEATING = "service_boiler_heating"

OpenLCCBiancaSensor = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaBinarySensor",
    cg.Component,
    cg.Parented.template(OpenLCCBianca),
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OpenLCCBiancaSensor),
        cv.GenerateID(CONF_OPEN_LCC_BIANCA_ID): cv.use_id(OpenLCCBianca),
        cv.Optional(CONF_BREWING): binary_sensor.binary_sensor_schema(
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_FILLING_SERVICE_BOILER): binary_sensor.binary_sensor_schema(
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_WATER_TANK_LOW): binary_sensor.binary_sensor_schema(
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_BREW_BOILER_HEATING): binary_sensor.binary_sensor_schema(
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_SERVICE_BOILER_HEATING): binary_sensor.binary_sensor_schema(
            icon=ICON_RADIOACTIVE,
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_OPEN_LCC_BIANCA_ID])

    if brewing_config := config.get(CONF_BREWING):
        sens = await binary_sensor.new_binary_sensor(brewing_config)
        cg.add(var.set_brewing(sens))
    if filling_config := config.get(CONF_FILLING_SERVICE_BOILER):
        sens = await binary_sensor.new_binary_sensor(filling_config)
        cg.add(var.set_filling_service_boiler(sens))
    if water_tank_low_config := config.get(CONF_WATER_TANK_LOW):
        sens = await binary_sensor.new_binary_sensor(water_tank_low_config)
        cg.add(var.set_water_tank_low(sens))
    if brew_boiler_heating_conf := config.get(CONF_BREW_BOILER_HEATING):
        sens = await binary_sensor.new_binary_sensor(brew_boiler_heating_conf)
        cg.add(var.set_brew_boiler_heating(sens))
    if service_boiler_heating_conf := config.get(CONF_SERVICE_BOILER_HEATING):
        sens = await binary_sensor.new_binary_sensor(service_boiler_heating_conf)
        cg.add(var.set_service_boiler_heating(sens))
