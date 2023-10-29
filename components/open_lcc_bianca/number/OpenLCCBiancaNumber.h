//
// Created by Magnus Nordlander on 2023-10-15.
//

#ifndef SMART_LCC_OPENLCCBIANCANUMBER_H
#define SMART_LCC_OPENLCCBIANCANUMBER_H

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/core/helpers.h"
#include "../esp-protocol.h"
#include "../OpenLCCBianca.h"
#include "LambdaNumber.h"

namespace esphome {
    namespace open_lcc_bianca {
        class OpenLCCBiancaNumber
                : public esphome::Component, public esphome::Parented<OpenLCCBianca> {
        public:
            void setup() {
                get_parent()->add_on_status_update_callback([this](ESPSystemStatusMessage msg) { this->handleStatus(msg); });
            }

            void handleStatus(ESPSystemStatusMessage message) {
                if (should_set(boiler_temp_offset_, message.brewTemperatureOffset)) {
                    boiler_temp_offset_->publish_state(message.brewTemperatureOffset);
                }

                if (should_set(coffee_boiler_set_point_, message.brewBoilerSetPoint)) {
                    coffee_boiler_set_point_->publish_state(message.brewBoilerSetPoint);
                }

                if (should_set(service_boiler_set_point_, message.serviceBoilerSetPoint)) {
                    service_boiler_set_point_->publish_state(message.serviceBoilerSetPoint);
                }

                if (should_set(auto_sleep_after_, message.autoSleepAfter)) {
                    auto_sleep_after_->publish_state(message.autoSleepAfter);
                }

            }

            void set_boiler_temp_offset(LambdaNumber *boiler_temp_offset) {
                boiler_temp_offset_ = boiler_temp_offset;
                boiler_temp_offset_->set_control_f([this](float state) {
                    get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_BREW_OFFSET, state);
                });
            }

            void set_coffee_boiler_set_point(LambdaNumber *coffee_boiler_set_point) {
                coffee_boiler_set_point_ = coffee_boiler_set_point;
                coffee_boiler_set_point_->set_control_f([this](float state) {
                    get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_BREW_SET_POINT, state);
                });
            }

            void set_service_boiler_set_point(LambdaNumber *service_boiler_set_point) {
                service_boiler_set_point_ = service_boiler_set_point;
                service_boiler_set_point_->set_control_f([this](float state) {
                    get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_SERVICE_SET_POINT, state);
                });
            }

            void set_auto_sleep_after(LambdaNumber *auto_sleep_after) {
                auto_sleep_after_ = auto_sleep_after;
                auto_sleep_after_->set_control_f([this](float state) {
                    get_parent()->sendCommand(ESP_SYSTEM_COMMAND_SET_AUTO_SLEEP_MINUTES, state);
                });
            }

            bool should_set(LambdaNumber *number, float value, float sigma = 0.1)
            {
                return number != nullptr && (!number->has_state() || fabs(number->state - value) >= sigma);
            }
        protected:
            LambdaNumber *boiler_temp_offset_{nullptr};
            LambdaNumber *coffee_boiler_set_point_{nullptr};
            LambdaNumber *service_boiler_set_point_{nullptr};
            LambdaNumber *auto_sleep_after_{nullptr};

        };
    }
}

#endif //SMART_LCC_OPENLCCBIANCANUMBER_H
