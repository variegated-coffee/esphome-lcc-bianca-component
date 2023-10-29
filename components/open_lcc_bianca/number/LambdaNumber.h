//
// Created by Magnus Nordlander on 2023-01-05.
//

#ifndef LCC_ESPHOME_LAMBDANUMBER_H
#define LCC_ESPHOME_LAMBDANUMBER_H

#include "esphome.h"
#include <functional>

namespace esphome {
    namespace open_lcc_bianca {

        class LambdaNumber : public esphome::number::Number {
        public:
            void control(float value) override {
                if (control_f_ != nullptr) {
                    this->control_f_(value);
                }
            }

            void set_control_f(std::function<void(float)> control_f) {
                control_f_ = std::move(control_f);
            }

        private:
            std::function<void(float)> control_f_{nullptr};
        };

    }
}

#endif //LCC_ESPHOME_LAMBDANUMBER_H
