//
// Created by Magnus Nordlander on 2023-10-10.
//

#ifndef SMART_LCC_AUTOMATION_H
#define SMART_LCC_AUTOMATION_H

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esp-protocol.h"
#include "OpenLCCBianca.h"

namespace esphome {
    namespace open_lcc_bianca {
        template<typename... Ts>
        class OpenLCCSetEcoModeOnAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetEcoModeOnAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_ECO_MODE, true); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetEcoModeOffAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetEcoModeOffAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_ECO_MODE, false); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetSleepModeOnAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetSleepModeOnAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_SLEEP_MODE, true); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetSleepModeOffAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetSleepModeOffAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_SLEEP_MODE, false); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCHardBailAction : public Action<Ts...> {
        public:
            explicit OpenLCCHardBailAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_FORCE_HARD_BAIL, false); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetBrewTemperatureOffsetAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetBrewTemperatureOffsetAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            TEMPLATABLE_VALUE(float, value)

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_BREW_OFFSET, this->value_.value(x...)); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetBrewBoilerSetPointAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetBrewBoilerSetPointAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            TEMPLATABLE_VALUE(float, value)

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_BREW_SET_POINT, this->value_.value(x...)); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetServiceBoilerSetPointAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetServiceBoilerSetPointAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            TEMPLATABLE_VALUE(float, value)

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_SERVICE_SET_POINT, this->value_.value(x...)); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCSetServiceAutoSleepAfterAction : public Action<Ts...> {
        public:
            explicit OpenLCCSetServiceAutoSleepAfterAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            TEMPLATABLE_VALUE(float, value)

            void play(Ts... x) override { bianca_->sendCommand(ESP_SYSTEM_COMMAND_SET_AUTO_SLEEP_MINUTES, this->value_.value(x...)); }

        protected:
            OpenLCCBianca *bianca_;
        };



        template<typename... Ts>
        class OpenLCCDisableAction : public Action<Ts...> {
        public:
            explicit OpenLCCDisableAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->disable(); }

        protected:
            OpenLCCBianca *bianca_;
        };

        template<typename... Ts>
        class OpenLCCEnableAction : public Action<Ts...> {
        public:
            explicit OpenLCCEnableAction(OpenLCCBianca *bianca) : bianca_(bianca) {}

            void play(Ts... x) override { bianca_->enable(); }

        protected:
            OpenLCCBianca *bianca_;
        };
    }
}

#endif //SMART_LCC_AUTOMATION_H
