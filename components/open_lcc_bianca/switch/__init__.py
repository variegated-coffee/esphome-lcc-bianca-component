import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_SECOND, ICON_RADIOACTIVE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE

from .. import open_lcc_bianca_ns, OpenLCCBianca

CONF_OPEN_LCC_BIANCA_ID = "open_lcc_bianca_id"

CONF_ECO_MODE = "eco_mode"
CONF_SLEEP_MODE = "sleep_mode"
CONF_LOW_FLOW_MODE = "low_flow_mode"

OpenLCCBiancaSwitch = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaSwitch",
    cg.Component,
    cg.Parented.template(OpenLCCBianca),
)

LambdaSwitch = open_lcc_bianca_ns.class_(
    "LambdaSwitch",
    switch.Switch,
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OpenLCCBiancaSwitch),
        cv.GenerateID(CONF_OPEN_LCC_BIANCA_ID): cv.use_id(OpenLCCBianca),
        cv.Optional(CONF_ECO_MODE): switch.switch_schema(
            class_=LambdaSwitch,
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_SLEEP_MODE): switch.switch_schema(
            class_=LambdaSwitch,
            icon=ICON_RADIOACTIVE,
        ),
        cv.Optional(CONF_LOW_FLOW_MODE): switch.switch_schema(
            class_=LambdaSwitch,
            icon=ICON_RADIOACTIVE,
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_OPEN_LCC_BIANCA_ID])

    if eco_mode_config := config.get(CONF_ECO_MODE):
        sw = await switch.new_switch(eco_mode_config)
        cg.add(var.set_eco_mode(sw))

    if sleep_mode_config := config.get(CONF_SLEEP_MODE):
        sw = await switch.new_switch(sleep_mode_config)
        cg.add(var.set_sleep_mode(sw))

    if low_flow_mode_config := config.get(CONF_LOW_FLOW_MODE):
        sw = await switch.new_switch(low_flow_mode_config)
        cg.add(var.set_low_flow_mode(sw))