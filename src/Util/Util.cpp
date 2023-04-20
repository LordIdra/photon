#include "Util.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

using std::ifstream;



auto Pow(const int x, const int y) -> int {
    // x^0 = 1
    if (y == 0) {
        return 1;
    }

    // Loop through y-1 times and multiply the current value by x
    int result = x;
    for (int i = 0; i < y-1; i++) {
        result *= x;
    }

    return result;
}

auto UnsignedBinaryToDenary(const vector<bool> &bits) -> int {
    int result = 0;
    int x = Pow(2, bits.size()-1);

    // Loop through every bit
    for (const bool &bit : bits) {
        if (bit) {
            result += x;
        }

        // Move on to the next power of two (128 -> 64, or 32 -> 16, etc)
        x /= 2;
    }

    return result;
}

auto SignedBinaryToDenary(const vector<bool> &bits) -> int {
    int result = UnsignedBinaryToDenary(bits);
    if (result > Pow(2, int(bits.size()-1))) {
        result -= Pow(2, bits.size());
    }
    return result;
}

auto UnsignedDenaryToBinary(int x, const int bits) -> vector<bool> {
    vector<bool> binary;

    // If x is greater than the maximum value for the given number of bits, that's an overflow
    // For the algorithm to work with this we'll need x >= 0 and x < max value
    int max_value = Pow(2, bits);
    if (x >= max_value) {
        x -= max_value;
    }

    // We'll start at the MSB, so find the value of said MSB
    int bit_value = max_value / 2;

    binary.reserve(bits);

    // Loop through every bit
    for (int bit = 0; bit < bits; bit++) {

        // Find if the current bit should be 1 or 0, add either 0 or 1 to the result string based on this
        // If the bit should be 1, subtract the bit value - this will allow us to use the same
        // technique on the next iteration
        if ((x - bit_value) >= 0) {
            binary.push_back(1); // NOLINT(modernize-use-bool-literals)
            x -= bit_value;
        } else {
            binary.push_back(0); // NOLINT(modernize-use-bool-literals)
        }

        // Next bit value will be half the previous one
        bit_value /= 2;
    }

    return binary;
}


auto SignedDenaryToBinary(int x, const int bits) -> vector<bool> {
    if (x < 0) {
        x += Pow(2, bits);
    }

    return UnsignedDenaryToBinary(x, bits);
}

auto UnsignedDenaryToBinaryString(int x, const int bits) -> string {
    vector<bool> binary = UnsignedDenaryToBinary(x, bits);
    string binary_string;
    for (const bool b : binary) {
        if (b) {
            binary_string.push_back('1');
        } else {
            binary_string.push_back('0');
        }
    }
    return binary_string;
}

auto Contains(const vector<int> &container, const int target) -> bool {
    return std::count(container.begin(), container.end(), target) != 0;
}

auto FormatValue(int x) -> string {
    if (x <= 9) {
        return "0" + std::to_string(x);
    }
    return std::to_string(x);
}

auto FormatAddress(int x) -> string {
    if (x <= 9) {
        return "000" + std::to_string(x);
    }

    if (x <= 99) {
        return "00" + std::to_string(x);
    }

    if (x <= 999) {
        return "0" + std::to_string(x);
    }
    
    return std::to_string(x);
}