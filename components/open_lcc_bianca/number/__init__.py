import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_SECOND, ICON_RADIOACTIVE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE

from .. import open_lcc_bianca_ns, OpenLCCBianca

CONF_OPEN_LCC_BIANCA_ID = "open_lcc_bianca_id"

CONF_BREW_BOILER_TEMP_OFFSET = "coffee_boiler_temperature_offset"
CONF_COFFEE_BOILER_TEMP_SET_POINT = "coffee_boiler_temperature_set_point"
CONF_SERVICE_BOILER_TEMP_SET_POINT = "service_boiler_temperature_set_point"
CONF_AUTO_SLEEP_AFTER = "auto_sleep_after"

OpenLCCBiancaNumber = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaNumber",
    cg.Component,
    cg.Parented.template(OpenLCCBianca),
)

LambdaNumber = open_lcc_bianca_ns.class_(
    "LambdaNumber",
    number.Number,
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OpenLCCBiancaNumber),
        cv.GenerateID(CONF_OPEN_LCC_BIANCA_ID): cv.use_id(OpenLCCBianca),
        cv.Optional(CONF_BREW_BOILER_TEMP_OFFSET): number.number_schema(
            class_=LambdaNumber,
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_COFFEE_BOILER_TEMP_SET_POINT): number.number_schema(
            class_=LambdaNumber,
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_SERVICE_BOILER_TEMP_SET_POINT): number.number_schema(
            class_=LambdaNumber,
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_AUTO_SLEEP_AFTER): number.number_schema(
            class_=LambdaNumber,
            icon=ICON_RADIOACTIVE,
        ),

    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_OPEN_LCC_BIANCA_ID])

    if status_config := config.get(CONF_BREW_BOILER_TEMP_OFFSET):
        sens = await number.new_number(
            status_config,
            min_value=-20,
            max_value=20,
            step=0.1
        )
        cg.add(var.set_boiler_temp_offset(sens))

    if status_config := config.get(CONF_COFFEE_BOILER_TEMP_SET_POINT):
        sens = await number.new_number(
            status_config,
            min_value=20,
            max_value=100,
            step=0.5
        )
        cg.add(var.set_coffee_boiler_set_point(sens))

    if status_config := config.get(CONF_SERVICE_BOILER_TEMP_SET_POINT):
        sens = await number.new_number(
            status_config,
            min_value=100,
            max_value=140,
            step=1
        )
        cg.add(var.set_service_boiler_set_point(sens))

    if status_config := config.get(CONF_AUTO_SLEEP_AFTER):
        sens = await number.new_number(
            status_config,
            min_value=0,
            max_value=600,
            step=1
        )
        cg.add(var.set_auto_sleep_after(sens))
