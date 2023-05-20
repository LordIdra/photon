#pragma once

#include "ComponentTest/ComponentTest.hpp"
#include <Util/Util.hpp>



namespace IntegrationTests {
    auto ResetTestPins() -> void;
    auto RunInstructions(const vector<pair<int, int>> &instructions) -> void;
}