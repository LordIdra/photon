#include "Assembler/Assembler.hpp"
#include <Util/Errors.hpp>

#include <iostream>
#include <stdexcept>
#include <string>



namespace Errors {
    namespace {
        auto Prefix(const string &file, const int line) -> string {
            return YELLOW + "[" + file + "|" + to_string(line) + "] ";
        }
    }

    namespace Files {
        NotFound::NotFound(const string &path) : std::runtime_error(
            RED + "File not found: " + CYAN + path + WHITE + "\n") {}
    }

    namespace Assembler {
        InvalidOpcode::InvalidOpcode(const string &file, const int line, const string &opcode) : std::runtime_error(
            Prefix(file, line) +
            RED + "Unknown opcode " + CYAN + opcode + WHITE + "\n") {}

        InvalidOperandCount::InvalidOperandCount(const string &file, const int line, const int expected, const int actual) : std::runtime_error(
            Prefix(file, line) +
            RED + "Expected " + CYAN + to_string(expected) + 
            RED + " operands but got " + CYAN + to_string(actual) + WHITE + "\n") {}
        
        InvalidOperandType::InvalidOperandType(const string &file, const int line, const string &operand) : std::runtime_error(
            Prefix(file, line) +
            RED + "Operand " + CYAN + operand + " is an invalid type" + WHITE + "\n") {}
        
        InvalidOperandValue::InvalidOperandValue(const string &file, const int line, const string &operand) : std::runtime_error(
            Prefix(file, line) +
            RED + "Invalid operand value " + CYAN + operand + WHITE + "\n") {}
        
        OperandOutOfBounds::OperandOutOfBounds(const string &file, const int line, const int operand) : std::runtime_error(
            Prefix(file, line) +
            RED + "Operand " + CYAN + to_string(operand) +
            RED + " out of bounds" + WHITE + "\n") {}
    }

    namespace Electronics {
        InvalidPinMode::InvalidPinMode() : std::runtime_error(
            RED + "Invalid pin mode"+ WHITE + "\n") {}
    }

    namespace TestBuilder {
        NothingToExpect::NothingToExpect(const string &file, const int line) : std::runtime_error(
            Prefix(file, line) +
            RED + "Nothing to expect"+ WHITE + "\n") {}
    }
}