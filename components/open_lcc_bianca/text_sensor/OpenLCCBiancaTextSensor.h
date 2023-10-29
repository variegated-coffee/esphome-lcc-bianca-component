//
// Created by Magnus Nordlander on 2023-10-10.
//

#ifndef SMART_LCC_OPENLCCBIANCATEXTSENSOR_H
#define SMART_LCC_OPENLCCBIANCATEXTSENSOR_H

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/helpers.h"
#include "../esp-protocol.h"
#include "../OpenLCCBianca.h"

namespace esphome {
    namespace open_lcc_bianca {
        class OpenLCCBiancaTextSensor
                : public esphome::Component, public esphome::Parented<OpenLCCBianca> {
        public:
            void setup() {
                get_parent()->add_on_status_update_callback([this](ESPSystemStatusMessage msg) { this->handleStatus(msg); });
            }

            void handleStatus(ESPSystemStatusMessage message) {
                auto stateString = prettifyCoalescedStateString(message.coalescedState);
                if (status_ != nullptr && (!status_->has_state() || status_->state != stateString)) {
                    status_->publish_state(stateString);
                }
            }

            std::string prettifyCoalescedStateString(ESPSystemCoalescedState state) {
                switch (state) {
                    case ESP_SYSTEM_COALESCED_STATE_UNDETERMINED:
                        return "Undetermined";
                    case ESP_SYSTEM_COALESCED_STATE_HEATUP:
                        return "Heatup";
                    case ESP_SYSTEM_COALESCED_STATE_TEMPS_NORMALIZING:
                        return "Temperatures normalizing";
                    case ESP_SYSTEM_COALESCED_STATE_WARM:
                        return "Warm";
                    case ESP_SYSTEM_COALESCED_STATE_SLEEPING:
                        return "Sleeping";
                    case ESP_SYSTEM_COALESCED_STATE_BAILED:
                        return "Bailed";
                    case ESP_SYSTEM_COALESCED_STATE_FIRST_RUN:
                        return "First run";
                }

                return "Unknown";
            }

            void set_status(esphome::text_sensor::TextSensor *status) { status_ = status; }
        protected:
            esphome::text_sensor::TextSensor *status_{nullptr};
        };
    }
}

#endif //SMART_LCC_OPENLCCBIANCATEXTSENSOR_H
