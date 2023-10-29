import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_SECOND, ICON_RADIOACTIVE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE

from .. import open_lcc_bianca_ns, OpenLCCBianca

CONF_OPEN_LCC_BIANCA_ID = "open_lcc_bianca_id"

CONF_STATUS = "status"

OpenLCCBiancaSensor = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaTextSensor",
    cg.Component,
    cg.Parented.template(OpenLCCBianca),
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(OpenLCCBiancaSensor),
        cv.GenerateID(CONF_OPEN_LCC_BIANCA_ID): cv.use_id(OpenLCCBianca),
        cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(
            icon=ICON_RADIOACTIVE,
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_OPEN_LCC_BIANCA_ID])

    if status_config := config.get(CONF_STATUS):
        sens = await text_sensor.new_text_sensor(status_config)
        cg.add(var.set_status(sens))
