import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_SECOND, ICON_RADIOACTIVE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE

from .. import open_lcc_bianca_ns, OpenLCCBianca

CONF_OPEN_LCC_BIANCA_ID = "open_lcc_bianca_id"

CONF_FLOW_MODE = "flow_mode"

OpenLCCBiancaSelect = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaSelect",
    cg.Component,
    cg.Parented.template(OpenLCCBianca),
)

LambdaSelect = open_lcc_bianca_ns.class_(
    "LambdaSelect",
    select.Select,
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OpenLCCBiancaSelect),
        cv.GenerateID(CONF_OPEN_LCC_BIANCA_ID): cv.use_id(OpenLCCBianca),
        cv.Optional(CONF_FLOW_MODE): select.select_schema(
            class_=LambdaSelect,
            icon=ICON_RADIOACTIVE,
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_OPEN_LCC_BIANCA_ID])

    if flow_mode_config := config.get(CONF_FLOW_MODE):
        sel = await select.new_select(flow_mode_config, options=["Full Flow", "Low Flow", "Line Pressure", "No Flow"])
        cg.add(var.set_flow_mode(sel))
