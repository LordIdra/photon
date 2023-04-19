#pragma once

#include "../Util/Types.hpp"



struct ComponentTestCase {
    unordered_map<string, int> input;
    unordered_map<string, int> expected_output;
    bool passed;
};