//
// Created by Magnus Nordlander on 2023-10-15.
//

#ifndef SMART_LCC_OPENLCCBIANCASWITCH_H
#define SMART_LCC_OPENLCCBIANCASWITCH_H

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/core/helpers.h"
#include "../esp-protocol.h"
#include "../OpenLCCBianca.h"
#include "LambdaSwitch.h"

namespace esphome {
    namespace open_lcc_bianca {
        class OpenLCCBiancaSwitch
                : public esphome::Component, public esphome::Parented<OpenLCCBianca> {
        public:
            void setup() {
                get_parent()->add_on_status_update_callback([this](ESPSystemStatusMessage msg) { this->handleStatus(msg); });
            }

            void handleStatus(ESPSystemStatusMessage message) {
                if (eco_mode_ != nullptr) {
                    eco_mode_->publish_state(message.ecoMode);
                }

                if (sleep_mode_ != nullptr) {
                    sleep_mode_->publish_state(message.sleepMode);
                }

                if (low_flow_mode_ != nullptr) {
                    low_flow_mode_->publish_state(message.flowMode != ESP_FLOW_MODE_FULL_FLOW);
                }
            }

            void set_eco_mode(LambdaSwitch *eco_mode) {
                eco_mode_ = eco_mode;
                eco_mode_->set_write_state_f([this](bool state) {
                    get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_ECO_MODE, state);
                });
            }

            void set_sleep_mode(LambdaSwitch *sleep_mode) {
                sleep_mode_ = sleep_mode;
                sleep_mode_->set_write_state_f([this](bool state) {
                    get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_SLEEP_MODE, state);
                });
            }

            void set_low_flow_mode(LambdaSwitch *low_flow_mode) {
                low_flow_mode_ = low_flow_mode;
                low_flow_mode_->set_write_state_f([this](bool state) {
                    if (state) {
                        get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_FLOW_MODE, (uint32_t)ESP_FLOW_MODE_PUMP_OFF_SOLENOID_OPEN);
                    } else {
                        get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_FLOW_MODE, (uint32_t)ESP_FLOW_MODE_FULL_FLOW);
                    }
                });
            }
        protected:
            LambdaSwitch *eco_mode_{nullptr};
            LambdaSwitch *sleep_mode_{nullptr};
            LambdaSwitch *low_flow_mode_{nullptr};
        };
    }
}

#endif //SMART_LCC_OPENLCCBIANCASWITCH_H
