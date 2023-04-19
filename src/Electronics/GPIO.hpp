#pragma once

#include "GPIO.hpp"
#include "Util/Util.hpp"



namespace GPIO {
    struct PinBlock {
        int starting_index;
        int pin_count;
        int mode;
    };

    auto SetupPinBlock(const PinBlock &block) -> void;
    auto Setup(const vector<PinBlock> &blocks) -> void;

    auto Set(const PinBlock &block, const int value) -> void;
    auto ReadBinary(const PinBlock &block) -> vector<bool>;
    auto ReadInt(const PinBlock &block) -> int;
}