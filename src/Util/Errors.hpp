#pragma once

#include <Util/Types.hpp>



// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
// https://en.wikipedia.org/wiki/ANSI_escape_code
namespace colors {
    const string WHITE = "\033[0;37m";
    const string CYAN  = "\033[0;36m";

    const string AMBER = "\033[0;33m";
    const string RED   = "\033[0;31m";

    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_RED   = "\033[1;31m";
}

namespace Errors {
    namespace Files {
        auto NotFound(const string &path) -> void;
    }

    namespace Asssembler {
        auto InvalidOpcode(const int line, const string &opcode) -> void;
        auto InvalidOperandCount(const int line, const int expected, const int actual) -> void;
        auto InvalidOperandType(const int line, const int expected, const int actual) -> void;
        auto InvalidOperand(const int line, const string &operand) -> void;
        auto OperandOutOfBounds(const int line, const int operand) -> void;
    }
    
    enum ErrorCode {
        FILES_NOT_FOUND,

        INVALID_OPCODE,
        INVALID_OPERAND_COUNT,
        INVALID_OPERAND_TYPE,
        INVALID_OPERAND,
        OPERAND_OUT_OF_BOUNDS,
    };

    
    auto Reset() -> void;
    auto HasErrorCode() -> bool;
    auto GetErrorCode() -> int;
    auto OutputErrors() -> void;
}