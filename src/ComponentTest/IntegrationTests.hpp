#pragma once

#include "ComponentTest/ComponentTest.hpp"
#include <Util/Util.hpp>



namespace IntegrationTests {
    auto ResetTestPins() -> void;
    auto RunInstructions(const bool debug_mode, const vector<pair<int, int>> &instructions) -> void;
}