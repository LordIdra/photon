#pragma once

#include "../Util/Types.hpp"
#include "Util/Util.hpp"



class TestNumber {
public:
    const int value;
    const bool is_signed;

    inline auto GetBinaryValue(const int bits) const -> vector<bool> {
        if (is_signed) {
            return SignedDenaryToBinary(value, bits);
        }
        return UnsignedDenaryToBinary(value, bits);
    }
};

struct ComponentTestCase {
    unordered_map<string, TestNumber> input;
    unordered_map<string, TestNumber> expected_output;
    unordered_map<string, TestNumber> actual_output;
    bool passed;
};
