#include "Util/Errors.hpp"
#include <cassert>
#include <catch2/catch_test_macros.hpp>

#include <Assembler/Assembler.hpp>
#include <Util/Util.hpp>
#include <stdexcept>



TEST_CASE("[2|ASM] Invalid opcode") {
    vector<string> tests {
        {"LOL BAYEH"},
        {"BYA 02 05"}};

    for (const string &t : tests) {
        try {
            Assembler::Assemble(0, t);
        } catch (Errors::Assembler::InvalidOpcode e) {
            return;
        } catch (const std::runtime_error &e) {}
        REQUIRE(false);
    }
}

TEST_CASE("[2|ASM] Invalid operand count") {
    vector<string> tests {
        {"ADD r3 r5 r9"},
        {"NOT"}};

    for (const string &t : tests) {
        try {
            Assembler::Assemble(0, t);
        } catch (Errors::Assembler::InvalidOperandCount &e) {
            return;
        } catch (const std::runtime_error &e) {}
        REQUIRE(false);
    }
}

TEST_CASE("[2|ASM] Invalid operand type") {
    vector<string> tests {
        {"ADC r10 50"},
        {"SET 20 50"},
        {"BIU r4"}};

    for (string t : tests) {
        try {
            Assembler::Assemble(0, t);
        } catch (const Errors::Assembler::InvalidOperandType &e) {
            return;
        } catch (const std::runtime_error &e) {}
        REQUIRE(false);
    }
}

TEST_CASE("[2|ASM] Invalid operand value") {
    vector<string> tests {
        {"ADC x 50"},
        {"SET y20 50"},
        {"BIU lol"}};

    for (string t : tests) {
        try {
            Assembler::Assemble(0, t);
        } catch (Errors::Assembler::InvalidOperandValue &e) {
            return;
        } catch (const std::runtime_error &e) {}
        REQUIRE(false);
    }
}

TEST_CASE("[2|ASM] Operand out of bounds") {
    vector<string> tests {
        {"ADC r5 r99"},
        {"SET -134 r3"},
        {"BIL -5"}};

    for (string t : tests) {
        try {
            Assembler::Assemble(0, t);
        } catch (Errors::Assembler::OperandOutOfBounds &e) {
            return;
        } catch (const std::runtime_error &e) {
            REQUIRE(false);
        }
        REQUIRE(false);
    }
}

TEST_CASE("[2|ASM] Valid instructions") {
    vector<pair<string, string>> tests {
        {"SET 2 r1", "240 33"},
        {"SET 87 r1", "245 113"},
        {"SET 2 r4", "240 36"},
        {"MOV r4 r1", "33 65"}};

    for (pair<string, string> t : tests) {
        CAPTURE(t.first, t.second);
        REQUIRE(Assembler::Assemble(0, t.first) == t.second);
    }
}