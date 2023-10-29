//
// Created by Magnus Nordlander on 2023-10-10.
//

#ifndef SMART_LCC_OPENLCCBIANCABINARYSENSOR_H
#define SMART_LCC_OPENLCCBIANCABINARYSENSOR_H

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/helpers.h"
#include "../esp-protocol.h"
#include "../OpenLCCBianca.h"

namespace esphome {
    namespace open_lcc_bianca {
        class OpenLCCBiancaBinarySensor
                : public esphome::Component, public esphome::Parented<OpenLCCBianca> {
        public:
            void setup() {
                get_parent()->add_on_status_update_callback([this](ESPSystemStatusMessage msg) { this->handleStatus(msg); });
            }

            void handleStatus(ESPSystemStatusMessage message) {
                if (brewing_ != nullptr) {
                    brewing_->publish_state(message.currentlyBrewing);
                }
                if (filling_service_boiler_ != nullptr) {
                    filling_service_boiler_->publish_state(message.currentlyFillingServiceBoiler);
                }
                if (water_tank_low_ != nullptr) {
                    water_tank_low_->publish_state(message.waterTankLow);
                }
                if (brew_boiler_heating_ != nullptr) {
                    brew_boiler_heating_->publish_state(message.brewBoilerOn);
                }
                if (service_boiler_heating_ != nullptr) {
                    service_boiler_heating_->publish_state(message.serviceBoilerOn);
                }
            }

            void set_brewing(esphome::binary_sensor::BinarySensor *brewing) { brewing_ = brewing; }
            void set_filling_service_boiler(esphome::binary_sensor::BinarySensor *filling_service_boiler) { filling_service_boiler_ = filling_service_boiler; }
            void set_water_tank_low(esphome::binary_sensor::BinarySensor *water_tank_low) { water_tank_low_ = water_tank_low; }
            void set_brew_boiler_heating(esphome::binary_sensor::BinarySensor *sens) { brew_boiler_heating_ = sens; }
            void set_service_boiler_heating(esphome::binary_sensor::BinarySensor *sens) { service_boiler_heating_ = sens; }
        protected:
            esphome::binary_sensor::BinarySensor *brewing_{nullptr};
            esphome::binary_sensor::BinarySensor *filling_service_boiler_{nullptr};
            esphome::binary_sensor::BinarySensor *water_tank_low_{nullptr};
            esphome::binary_sensor::BinarySensor *brew_boiler_heating_{nullptr};
            esphome::binary_sensor::BinarySensor *service_boiler_heating_{nullptr};
        };
    }
}

#endif //SMART_LCC_OPENLCCBIANCABINARYSENSOR_H
