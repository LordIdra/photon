#include "Assembler/Assembler.hpp"
#include <Util/Errors.hpp>

#include <iostream>
#include <stdexcept>
#include <string>



namespace Errors {
    namespace Files {
        NotFound::NotFound(const string &path) : std::runtime_error(
            Colors::RED + "File not found: " + Colors::CYAN + path + "\n") {}
    }

    namespace Assembler {
        InvalidOpcode::InvalidOpcode(const int line, const string &opcode) : std::runtime_error(
            Colors::AMBER + "[" + to_string(line) + "] " +
            Colors::RED + "Unknown opcode " + Colors::CYAN + opcode + Colors::WHITE) {}

        InvalidOperandCount::InvalidOperandCount(const int line, const int expected, const int actual) : std::runtime_error(
            Colors::AMBER + "[" + to_string(line) + "] " +
            Colors::RED + "Expected " + Colors::CYAN + to_string(expected) + 
            Colors::RED + " operands but got " + Colors::CYAN + to_string(actual) + Colors::WHITE) {}
        
        InvalidOperandType::InvalidOperandType(const int line, const string &operand) : std::runtime_error(
            Colors::AMBER + "[" + to_string(line) + "] " +
            Colors::RED + "Operand " + Colors::CYAN + operand + " is an invalid type" + Colors::WHITE) {}
        
        InvalidOperandValue::InvalidOperandValue(const int line, const string &operand) : std::runtime_error(
            Colors::AMBER + "[" + to_string(line) + "] " +
            Colors::RED + "Invalid operand value " + Colors::CYAN + operand + Colors::WHITE) {}
        
        OperandOutOfBounds::OperandOutOfBounds(const int line, const int operand) : std::runtime_error(
            Colors::AMBER + "[" + to_string(line) + "] " +
            Colors::RED + "Operand " + Colors::CYAN + to_string(operand) +
            Colors::RED + " out of bounds" + Colors::WHITE) {}
    }
}