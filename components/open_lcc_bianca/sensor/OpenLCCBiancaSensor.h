//
// Created by Magnus Nordlander on 2023-09-30.
//

#ifndef SMART_LCC_OPENLCCBIANCASENSOR_H
#define SMART_LCC_OPENLCCBIANCASENSOR_H

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/helpers.h"
#include "../esp-protocol.h"
#include "../OpenLCCBianca.h"

namespace esphome {
    namespace open_lcc_bianca {
        class OpenLCCBiancaSensor
                : public esphome::Component, public esphome::Parented<OpenLCCBianca> {
        public:
            void setup() {
                get_parent()->add_on_status_update_callback([this](ESPSystemStatusMessage msg) { this->handleStatus(msg); });
            }

            void handleStatus(ESPSystemStatusMessage message) {
                if (should_set(coffee_boiler_temperature_, message.brewBoilerTemperature)) {
                    coffee_boiler_temperature_->publish_state(message.brewBoilerTemperature);
                }
                if (should_set(service_boiler_temperature_, message.serviceBoilerTemperature)) {
                    service_boiler_temperature_->publish_state(message.serviceBoilerTemperature);
                }
                if (should_set(auto_sleep_in_, message.plannedAutoSleepInSeconds)) {
                    auto_sleep_in_->publish_state(message.plannedAutoSleepInSeconds);
                }
                if (should_set(rp2040_uptime_, message.rp2040UptimeSeconds)) {
                    rp2040_uptime_->publish_state(message.rp2040UptimeSeconds);
                }
                if (should_set(external_temperature_1_, message.externalTemperature1)) {
                    external_temperature_1_->publish_state(message.externalTemperature1);
                }
                if (should_set(external_temperature_2_, message.externalTemperature2)) {
                    external_temperature_2_->publish_state(message.externalTemperature2);
                }
                if (should_set(external_temperature_3_, message.externalTemperature3)) {
                    external_temperature_3_->publish_state(message.externalTemperature3);
                }

                if (brew_time_ != nullptr && !brew_time_->has_state()) {
                    brew_time_->publish_state(0.f);
                }

                if (message.currentlyBrewing) {
                    if (!wasBrewing) {
                        latestBrewStart = millis();
                        wasBrewing = true;
                    }

                    float diff = (float)(millis() - latestBrewStart) / 1000.f;

                    if (should_set(brew_time_, diff)) {
                        brew_time_->publish_state(diff);
                    }
                } else {
                    wasBrewing = false;
                }
            }

            bool should_set(esphome::sensor::Sensor *sensor, float value, float sigma = 0.1)
            {
                return sensor != nullptr && (!sensor->has_state() || fabs(sensor->get_state() - value) >= sigma);
            }

            void set_coffee_boiler_temperature(esphome::sensor::Sensor *coffee_boiler_temperature) { coffee_boiler_temperature_ = coffee_boiler_temperature; }
            void set_service_boiler_temperature(esphome::sensor::Sensor *service_boiler_temperature) { service_boiler_temperature_ = service_boiler_temperature; }
            void set_auto_sleep_in(esphome::sensor::Sensor *auto_sleep_in) { auto_sleep_in_ = auto_sleep_in; }
            void set_rp2040_uptime(esphome::sensor::Sensor *rp2040_uptime) { rp2040_uptime_ = rp2040_uptime; }
            void set_external_temperature_1(esphome::sensor::Sensor *sens) { external_temperature_1_ = sens; }
            void set_external_temperature_2(esphome::sensor::Sensor *sens) { external_temperature_2_ = sens; }
            void set_external_temperature_3(esphome::sensor::Sensor *sens) { external_temperature_3_ = sens; }
            void set_brew_time(esphome::sensor::Sensor *sens) { brew_time_ = sens; }

        protected:
            esphome::sensor::Sensor *coffee_boiler_temperature_{nullptr};
            esphome::sensor::Sensor *service_boiler_temperature_{nullptr};
            esphome::sensor::Sensor *auto_sleep_in_{nullptr};
            esphome::sensor::Sensor *rp2040_uptime_{nullptr};
            esphome::sensor::Sensor *external_temperature_1_{nullptr};
            esphome::sensor::Sensor *external_temperature_2_{nullptr};
            esphome::sensor::Sensor *external_temperature_3_{nullptr};
            esphome::sensor::Sensor *brew_time_{nullptr};

            bool wasBrewing = false;
            uint32_t latestBrewStart = 0;
        };
    }
}

#endif //SMART_LCC_OPENLCCBIANCASENSOR_H
