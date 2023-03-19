#include "Assembler/Assembler.hpp"
#include <Util/Errors.hpp>

#include <iostream>
#include <stdexcept>
#include <string>



namespace Errors {
    namespace {
        auto Prefix(const string &file, const int line) -> string {
            return Colors::AMBER + "[" + file + "|" + to_string(line) + "] ";
        }
    }

    namespace Files {
        NotFound::NotFound(const string &path) : std::runtime_error(
            Colors::RED + "File not found: " + Colors::CYAN + path + Colors::WHITE + "\n") {}
    }

    namespace Assembler {
        InvalidOpcode::InvalidOpcode(const string &file, const int line, const string &opcode) : std::runtime_error(
            Prefix(file, line) +
            Colors::RED + "Unknown opcode " + Colors::CYAN + opcode + Colors::WHITE + "\n") {}

        InvalidOperandCount::InvalidOperandCount(const string &file, const int line, const int expected, const int actual) : std::runtime_error(
            Prefix(file, line) +
            Colors::RED + "Expected " + Colors::CYAN + to_string(expected) + 
            Colors::RED + " operands but got " + Colors::CYAN + to_string(actual) + Colors::WHITE + "\n") {}
        
        InvalidOperandType::InvalidOperandType(const string &file, const int line, const string &operand) : std::runtime_error(
            Prefix(file, line) +
            Colors::RED + "Operand " + Colors::CYAN + operand + " is an invalid type" + Colors::WHITE + "\n") {}
        
        InvalidOperandValue::InvalidOperandValue(const string &file, const int line, const string &operand) : std::runtime_error(
            Prefix(file, line) +
            Colors::RED + "Invalid operand value " + Colors::CYAN + operand + Colors::WHITE + "\n") {}
        
        OperandOutOfBounds::OperandOutOfBounds(const string &file, const int line, const int operand) : std::runtime_error(
            Prefix(file, line) +
            Colors::RED + "Operand " + Colors::CYAN + to_string(operand) +
            Colors::RED + " out of bounds" + Colors::WHITE + "\n") {}
    }

    namespace TestBuilder {
        NothingToExpect::NothingToExpect(const string &file, const int line) : std::runtime_error(
            Prefix(file, line) +
            Colors::RED + "Nothing to expect"+ Colors::WHITE + "\n") {}
    }
}