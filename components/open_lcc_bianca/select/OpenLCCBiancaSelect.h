//
// Created by Magnus Nordlander on 2023-11-16.
//

#ifndef ESPHOME_BIANCA_OPENLCCBIANCASELECT_H
#define ESPHOME_BIANCA_OPENLCCBIANCASELECT_H
#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "esphome/core/helpers.h"
#include "../esp-protocol.h"
#include "../OpenLCCBianca.h"
#include "LambdaSelect.h"

namespace esphome {
    namespace open_lcc_bianca {
        class OpenLCCBiancaSelect
                : public esphome::Component, public esphome::Parented<OpenLCCBianca> {
        public:
            void setup() {
                get_parent()->add_on_status_update_callback([this](ESPSystemStatusMessage msg) { this->handleStatus(msg); });
            }

            void handleStatus(ESPSystemStatusMessage message) {
                if (flow_mode_ != nullptr) {
                    std::string flowString;

//                    ESP_LOGD("LCC", "Flow mode %u", message.flowMode);

                    switch (message.flowMode) {
                        case ESP_FLOW_MODE_PUMP_ON_SOLENOID_OPEN:
                            flowString = "Full Flow";
                            break;
                        case ESP_FLOW_MODE_PUMP_ON_SOLENOID_CLOSED:
                            flowString = "Low Flow";
                            break;
                        case ESP_FLOW_MODE_PUMP_OFF_SOLENOID_OPEN:
                            flowString = "Line Pressure";
                            break;
                        case ESP_FLOW_MODE_PUMP_OFF_SOLENOID_CLOSED:
                            flowString = "No Flow";
                            break;
                        default:
                            ESP_LOGW("LCC", "Unknown flow mode");
                            return;
                    }

                    if (!flow_mode_->has_state() || flow_mode_->state != flowString) {
                        flow_mode_->publish_state(flowString);
                    }
                }
            }

            void set_flow_mode(LambdaSelect *flow_mode) {
                flow_mode_ = flow_mode;
                flow_mode_->set_control_f([this](const std::string& str) {
                    if (str == "Full Flow") {
                        get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_FLOW_MODE, (uint32_t)ESP_FLOW_MODE_PUMP_ON_SOLENOID_OPEN);
                    } else if (str == "Low Flow") {
                        get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_FLOW_MODE, (uint32_t)ESP_FLOW_MODE_PUMP_ON_SOLENOID_CLOSED);
                    } else if (str == "Line Pressure") {
                        get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_FLOW_MODE, (uint32_t)ESP_FLOW_MODE_PUMP_OFF_SOLENOID_OPEN);
                    } else if (str == "No Flow") {
                        get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_FLOW_MODE, (uint32_t)ESP_FLOW_MODE_PUMP_OFF_SOLENOID_CLOSED);
                    } else {
                        ESP_LOGW("LCC", "Unknown flow mode");
                    }
                });
            }
        protected:
            LambdaSelect *flow_mode_{nullptr};
        };
    }
}

#endif //ESPHOME_BIANCA_OPENLCCBIANCASELECT_H
