#pragma once

#include "ComponentTestCase.hpp"

#include <Electronics/GPIO.hpp>



namespace ComponentTestPrint {
    auto LoopTestResults(const vector<ComponentTestCase> &tests, const unordered_map<string, GPIO::PinBlock> &pin_blocks) -> void;
}