import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.components import uart
from esphome.const import (
    CONF_ID,
    CONF_TRIGGER_ID,
)

CODEOWNERS = ["@magnusnordlander"]
open_lcc_bianca_ns = cg.esphome_ns.namespace("esphome::open_lcc_bianca")

OpenLCCBianca = open_lcc_bianca_ns.class_("OpenLCCBianca", cg.Component, uart.UARTDevice)

OpenLCCEnableAction = open_lcc_bianca_ns.class_("OpenLCCEnableAction", automation.Action)
OpenLCCDisableAction = open_lcc_bianca_ns.class_("OpenLCCDisableAction", automation.Action)

OpenLCCSetEcoModeOnAction = open_lcc_bianca_ns.class_("OpenLCCSetEcoModeOnAction", automation.Action)
OpenLCCSetEcoModeOffAction = open_lcc_bianca_ns.class_("OpenLCCSetEcoModeOffAction", automation.Action)
OpenLCCSetSleepModeOnAction = open_lcc_bianca_ns.class_("OpenLCCSetSleepModeOnAction", automation.Action)
OpenLCCSetSleepModeOffAction = open_lcc_bianca_ns.class_("OpenLCCSetSleepModeOffAction", automation.Action)
OpenLCCSetBrewTemperatureOffsetAction = open_lcc_bianca_ns.class_("OpenLCCSetBrewTemperatureOffsetAction", automation.Action)
OpenLCCSetServiceAutoSleepAfterAction = open_lcc_bianca_ns.class_("OpenLCCSetServiceAutoSleepAfterAction", automation.Action)
OpenLCCSetBrewBoilerSetPointAction = open_lcc_bianca_ns.class_("OpenLCCSetBrewBoilerSetPointAction", automation.Action)
OpenLCCSetServiceBoilerSetPointAction = open_lcc_bianca_ns.class_("OpenLCCSetServiceBoilerSetPointAction", automation.Action)
OpenLCCHardBailAction = open_lcc_bianca_ns.class_("OpenLCCHardBailAction", automation.Action)
StatusMessageType = cg.RawExpression("ESPSystemStatusMessage")

OpenLCCBiancaOnStatusUpdateTrigger = open_lcc_bianca_ns.class_(
    "OpenLCCBiancaOnStatusUpdateTrigger", automation.Trigger.template(StatusMessageType)
)

CONF_LCC_ID = "lcc_id"
CONF_ON_STATUS_UPDATE = "on_status_update"
CONF_VALUE = "value"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(OpenLCCBianca),
        cv.Optional(CONF_ON_STATUS_UPDATE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OpenLCCBiancaOnStatusUpdateTrigger),
            }
        ),
    }
).extend(uart.UART_DEVICE_SCHEMA)

OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.templatable(cv.use_id(OpenLCCBianca)),
    }
)

OPENLCC_BIANCA_ACTION_SINGLE_FLOAT_ARG_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.templatable(cv.use_id(OpenLCCBianca)),
        cv.Required(CONF_VALUE): cv.templatable(
            cv.float_
        ),
    }
)

OPENLCC_BIANCA_ACTION_SINGLE_UINT16_ARG_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.templatable(cv.use_id(OpenLCCBianca)),
        cv.Required(CONF_VALUE): cv.templatable(
            cv.uint16_t
        ),
    }
)

@automation.register_action("open_lcc.bianca.enable", OpenLCCEnableAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.disable", OpenLCCDisableAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_eco_mode_on", OpenLCCSetEcoModeOnAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_eco_mode_off", OpenLCCSetEcoModeOffAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_sleep_mode_on", OpenLCCSetSleepModeOnAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_sleep_mode_off", OpenLCCSetSleepModeOffAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.hard_bail", OpenLCCHardBailAction, OPENLCC_BIANCA_ACTION_NO_ARG_SCHEMA)
async def no_arg_action(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

@automation.register_action("open_lcc.bianca.set_auto_sleep_after", OpenLCCSetServiceAutoSleepAfterAction, OPENLCC_BIANCA_ACTION_SINGLE_FLOAT_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_brew_temperature_offset", OpenLCCSetBrewTemperatureOffsetAction, OPENLCC_BIANCA_ACTION_SINGLE_FLOAT_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_brew_boiler_set_point", OpenLCCSetBrewBoilerSetPointAction, OPENLCC_BIANCA_ACTION_SINGLE_FLOAT_ARG_SCHEMA)
@automation.register_action("open_lcc.bianca.set_service_boiler_set_point", OpenLCCSetServiceBoilerSetPointAction, OPENLCC_BIANCA_ACTION_SINGLE_FLOAT_ARG_SCHEMA)
async def value_float_arg_action(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_VALUE], args, float)
    cg.add(var.set_value(template_))
    return var

#async def value_uint16_arg_action(config, action_id, template_arg, args):
#    paren = await cg.get_variable(config[CONF_ID])
#    var = cg.new_Pvariable(action_id, template_arg, paren)
#    template_ = await cg.templatable(config[CONF_VALUE], args, cg.uint16)
#    cg.add(var.set_value(template_))
#    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    for conf in config.get(CONF_ON_STATUS_UPDATE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(StatusMessageType, "msg")], conf)
