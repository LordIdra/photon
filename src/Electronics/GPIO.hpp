#pragma once

#include "GPIO.hpp"
#include "Util/Util.hpp"
#include "ComponentTest/ComponentTestCase.hpp"



namespace GPIO {
    struct PinBlock {
        int starting_index;
        int pin_count;
        int mode;
    };

    auto SetupWiringPi() -> void;
    auto SetupPinBlock(const PinBlock &block) -> void;
    auto Setup(const vector<PinBlock> &blocks) -> void;

    auto Set(const PinBlock &block, const TestNumber &value) -> void;
    auto ReadBinary(const PinBlock &block) -> vector<bool>;
    auto ReadInt(const PinBlock &block, const bool is_signed) -> int;
}