//
// Created by Magnus Nordlander on 2023-11-16.
//

#ifndef ESPHOME_BIANCA_LAMBDASELECT_H
#define ESPHOME_BIANCA_LAMBDASELECT_H

#include "esphome.h"
#include <functional>

namespace esphome {
    namespace open_lcc_bianca {

        class LambdaSelect : public esphome::select::Select {
        public:
            void control(const std::string &value) override {
                if (control_f_ != nullptr) {
                    this->control_f_(value);
                }
            }

            void set_control_f(std::function<void(const std::string &value)> control_f) {
                control_f_ = std::move(control_f);
            }

        private:
            std::function<void(const std::string &value)> control_f_{nullptr};
        };

    }
}

#endif //ESPHOME_BIANCA_LAMBDASELECT_H
