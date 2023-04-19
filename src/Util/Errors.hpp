#pragma once

#include <Util/Types.hpp>
#include <Util/Colors.hpp>



// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
// https://en.wikipedia.org/wiki/ANSI_escape_code
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
            InvalidOpcode(const string &file, const int line, const string &opcode);
        };

        class InvalidOperandCount : public std::runtime_error {
        public:
            InvalidOperandCount(const string &file, const int line, const int expected, const int actual);
        };

        class InvalidOperandType : public std::runtime_error {
        public:
            InvalidOperandType(const string &file, const int line, const string &operand);
        };

        class InvalidOperandValue : public std::runtime_error {
        public:
            InvalidOperandValue(const string &file, const int line, const string &operand);
        };

        class OperandOutOfBounds : public std::runtime_error {
        public:
            OperandOutOfBounds(const string &file, const int line, const int operand);
        };
    }

    namespace Electronics {
        class InvalidPinMode : public std::runtime_error {
        public:
            InvalidPinMode();
        };
    }

    namespace TestBuilder {
        class NothingToExpect : public std::runtime_error {
        public:
            NothingToExpect(const string &file, const int line);
        };
    }
}