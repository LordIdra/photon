#include "GPIO.hpp"

#include "../Util/Errors.hpp"
#include <wiringPi.h>



namespace GPIO {
    namespace {
        vector<int> pin_identifiers = {
            8, 9, 7, 0, 2, 3, 12, 13, 14, 30, 21, 22, 23, 24, 25,
            15, 16, 1, 4, 5, 6, 10, 11, 31, 26, 27, 28, 29,
        };
    }



    auto SetupPinBlock(const PinBlock &block) -> void {
        for (int pin = 0; pin < block.pin_count; pin++) {
            pinMode(pin_identifiers.at(block.starting_index + pin), block.mode);
        }
    }

    auto Setup(const vector<PinBlock> &blocks) -> void {
        wiringPiSetup();
        for (const PinBlock &block : blocks) {
            SetupPinBlock(block);
        }
    }



    auto Set(const PinBlock &block, const int value) -> void {
        if (block.mode != OUTPUT) {
            throw Errors::Electronics::InvalidPinMode();
        }
        vector<bool> bits = SignedDenaryToBinary(value, block.pin_count);
        for (int pin = 0; pin < block.pin_count; pin++) {
            digitalWrite(pin_identifiers.at(block.starting_index + pin), bits[pin]);
        }
    }

    auto ReadBinary(const PinBlock &block) -> vector<bool> {
        if (block.mode != INPUT) {
            throw Errors::Electronics::InvalidPinMode();
        }
        vector<bool> bits;
        bits.reserve(block.pin_count);
        for (int pin = 0; pin < block.pin_count; pin++) {
            bits.push_back(digitalRead(pin_identifiers.at(block.starting_index + pin)));
        }
        return bits;
    }

    auto ReadInt(const PinBlock &block) -> int {
        return SignedBinaryToDenary(ReadBinary(block));
    }
}