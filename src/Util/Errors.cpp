#include "Assembler/Assembler.hpp"
#include <Util/Errors.hpp>

#include <iostream>
#include <string>



namespace Errors {

    namespace {
        optional<int> errorCode;

        auto AddError(const ErrorCode code, const string &error) -> void {
            cout << error << colors::WHITE << "\n";
            errorCode = code;
        }
    }

    namespace Files {
        auto NotFound(const string &path) -> void {
            AddError(Errors::FILES_NOT_FOUND, colors::RED + "File not found: " + colors::CYAN + path + "\n");
        }
    }

    namespace Assembler {
        auto InvalidOpcode(const int line, const string &opcode) -> void {
            AddError(INVALID_OPCODE, 
                colors::AMBER + "[" + to_string(line) + "] " +
                colors::RED + "Unknown opcode " + colors::CYAN + opcode + colors::WHITE);
        }

        auto InvalidOperandCount(const int line, const int expected, const int actual) -> void {
            AddError(INVALID_OPERAND_COUNT, 
                colors::AMBER + "[" + to_string(line) + "] " +
                colors::RED + "Expected " + colors::CYAN + to_string(expected) + 
                colors::RED + " operands but got " + colors::CYAN + to_string(actual) + colors::WHITE);
        }

        auto InvalidOperandType(const int line, const string &operand) -> void {
            AddError(INVALID_OPERAND_TYPE, 
                colors::AMBER + "[" + to_string(line) + "] " +
                colors::RED + "Operand " + colors::CYAN + operand + " is an invalid type" + colors::WHITE);
        }

        auto InvalidOperand(const int line, const string &operand) -> void {
            AddError(INVALID_OPERAND, 
                colors::AMBER + "[" + to_string(line) + "] " +
                colors::RED + "Invalid operand " + colors::CYAN + operand + colors::WHITE);
        }

        auto OperandOutOfBounds(const int line, const int operand) -> void {
            AddError(OPERAND_OUT_OF_BOUNDS, 
                colors::AMBER + "[" + to_string(line) + "] " +
                colors::RED + "Operand " + colors::CYAN + to_string(operand) +
                colors::RED + " out of bounds" + colors::WHITE);
        }
    }

    auto Reset() -> void {
        errorCode.reset();
    }

    auto HasErrorCode() -> bool {
        return errorCode.has_value();
    }

    auto GetErrorCode() -> int {
        return errorCode.value();
    }
}