#include "Util/Errors.hpp"
#include <cassert>
#include <catch2/catch_test_macros.hpp>

#include <Assembler/Assembler.hpp>
#include <Util/Util.hpp>
#include <stdexcept>



TEST_CASE("[ASM] Invalid opcode") {
    vector<string> tests {
        {"LOL BAYEH"},
        {"BYA 02 05"}};

    for (const string &t : tests) {
        REQUIRE_THROWS_AS(Assembler::AssembleToString("TEST", 0, t), Errors::Assembler::InvalidOpcode);
    }
}

TEST_CASE("[ASM] Invalid operand count") {
    vector<string> tests {
        {"ADD r3 r5 r9"},
        {"NOT"}};

    for (const string &t : tests) {
        REQUIRE_THROWS_AS(Assembler::AssembleToString("TEST", 0, t), Errors::Assembler::InvalidOperandCount);
    }
}

TEST_CASE("[ASM] Invalid operand type") {
    vector<string> tests {
        {"ADC r10 50"},
        {"SET 20 50"},
        {"BIU r4"}};

    for (const string &t : tests) {
        REQUIRE_THROWS_AS(Assembler::AssembleToString("TEST", 0, t), Errors::Assembler::InvalidOperandType);
    }
}

TEST_CASE("[ASM] Invalid operand value") {
    vector<string> tests {
        {"ADC x 50"},
        {"SET y20 50"},
        {"BIU lol"}};

    for (const string &t : tests) {
        REQUIRE_THROWS_AS(Assembler::AssembleToString("TEST", 0, t), Errors::Assembler::InvalidOperandValue);
    }
}

TEST_CASE("[ASM] Operand out of bounds") {
    vector<string> tests {
        {"ADC r5 r99"},
        {"SET -134 r3"},
        {"BIL -5"}};

    for (const string &t : tests) {
        REQUIRE_THROWS_AS(Assembler::AssembleToString("TEST", 0, t), Errors::Assembler::OperandOutOfBounds);
    }
}

TEST_CASE("[ASM] Valid instructions") {
    vector<pair<string, string>> tests {
        {"SET 2 r1", "240 33"},
        {"SET 87 r1", "245 113"},
        {"SET 2 r4", "240 36"},
        {"MOV r1 r4", "33 65"}};

    for (pair<string, string> t : tests) {
        CAPTURE(t.first, t.second);
        REQUIRE(Assembler::AssembleToString("TEST", 0, t.first) == t.second);
    }
}