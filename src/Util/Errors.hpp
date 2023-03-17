#pragma once

#include <Util/Types.hpp>



// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
// https://en.wikipedia.org/wiki/ANSI_escape_code
namespace Colors {
    const string WHITE = "\033[0;37m";
    const string CYAN  = "\033[0;36m";

    const string AMBER = "\033[0;33m";
    const string RED   = "\033[0;31m";

    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_RED   = "\033[1;31m";
}

namespace Errors {
    namespace Files {
        class NotFound : public std::runtime_error {
        public:
            NotFound(const string &path);
        };
    }

    namespace Assembler {
        class InvalidOpcode : public std::runtime_error {
        public:
            InvalidOpcode(const int line, const string &opcode);
        };

        class InvalidOperandCount : public std::runtime_error {
        public:
            InvalidOperandCount(const int line, const int expected, const int actual);
        };

        class InvalidOperandType : public std::runtime_error {
        public:
            InvalidOperandType(const int line, const string &operand);
        };

        class InvalidOperandValue : public std::runtime_error {
        public:
            InvalidOperandValue(const int line, const string &operand);
        };

        class OperandOutOfBounds : public std::runtime_error {
        public:
            OperandOutOfBounds(const int line, const int operand);
        };
    }
}