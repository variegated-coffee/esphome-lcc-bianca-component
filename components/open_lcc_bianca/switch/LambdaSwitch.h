//
// Created by Magnus Nordlander on 2023-01-05.
//

#ifndef LCC_ESPHOME_LAMBDASWITCH_H
#define LCC_ESPHOME_LAMBDASWITCH_H

#include "esphome.h"
#include "esphome/components/switch/switch.h"
#include <functional>


namespace esphome {
    namespace open_lcc_bianca {
        class LambdaSwitch : public esphome::switch_::Switch {
        public:
            void write_state(bool state) override {
                if (write_state_f_ != nullptr) {
                    this->write_state_f_(state);
                }
            }

            void set_write_state_f(std::function<void(bool)> write_state_f) {
                write_state_f_ = std::move(write_state_f);
            }

        private:
            std::function<void(bool)> write_state_f_{nullptr};
        };

    }
}

#endif //LCC_ESPHOME_LAMBDASWITCH_H
