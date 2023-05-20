#pragma once

#include <unordered_map>
#include <Util/Types.hpp>



namespace Assembler {
    const char COMMENT_CHARACTER = ';';

    const int WORD_SIZE = 8;

    const int MIN_REGISTER = 0;
    const int MAX_REGISTER = 15;
    const int MIN_ADDRESS = 0;
    const int MAX_ADDRESS = 4095;
    const int MIN_VALUE = -128;
    const int MAX_VALUE = 255; // Max for unsigned is 255, for signed 127

    enum OperandType {
        REGISTER,
        VALUE,
    };

    const unordered_map<string, int> OPCODES = {
        {"ADD", 8}, 
        {"ADC", 24}, 
        {"SUB", 40}, 
        {"SBC", 56}, 
        {"INC", 72}, 
        {"DEC", 88},
        {"INV", 104},

        {"XOR", 4}, 
        {"AND", 20}, 
        {"ORR", 36}, 
        {"NOT", 52}, 

        {"LDD", 2}, 
        {"STD", 18}, 
        {"BDU", 34}, 
        {"BDG", 50},
        {"BDL", 66},
        {"BDE", 82}, 
        {"BDN", 98}, 

        {"NOP", 1}, 
        {"HLT", 17}, 
        {"MOV", 33}, 
        {"MTD", 49},
        {"MFD", 65},
        {"CMP", 81}, 
        {"TST", 97}, 

        {"LDI", 128}, 
        {"STI", 144}, 
        {"BIU", 160}, 
        {"BIG", 176},
        {"BIL", 192},
        {"BIE", 208}, 
        {"BIN", 224}, 
        {"SET", 240}, 
    };

    const unordered_map<string, vector<OperandType>> OPERANDS = {
        {"ADD", {REGISTER, REGISTER}}, 
        {"ADC", {REGISTER, REGISTER}}, 
        {"SUB", {REGISTER, REGISTER}}, 
        {"SBC", {REGISTER, REGISTER}}, 
        {"INC", {REGISTER}}, 
        {"DEC", {REGISTER}},
        {"INV", {REGISTER, REGISTER}},

        {"XOR", {REGISTER, REGISTER}}, 
        {"AND", {REGISTER, REGISTER}}, 
        {"ORR", {REGISTER, REGISTER}}, 
        {"NOT", {REGISTER, REGISTER}},

        {"LDD", {REGISTER, REGISTER}}, 
        {"STD", {REGISTER, REGISTER}}, 
        {"BDU", {REGISTER, REGISTER}}, 
        {"BDG", {REGISTER, REGISTER}},
        {"BDL", {REGISTER, REGISTER}},
        {"BDE", {REGISTER, REGISTER}}, 
        {"BDN", {REGISTER, REGISTER}}, 

        {"NOP", {}}, 
        {"HLT", {}}, 
        {"MOV", {REGISTER, REGISTER}}, 
        {"MTD", {REGISTER}},
        {"MFD", {REGISTER}},
        {"CMP", {REGISTER, REGISTER}}, 
        {"TST", {REGISTER}}, 

        {"LDI", {VALUE}}, 
        {"STI", {VALUE}}, 
        {"BIU", {VALUE}}, 
        {"BIG", {VALUE}},
        {"BIL", {VALUE}},
        {"BIE", {VALUE}}, 
        {"BIN", {VALUE}}, 
        {"SET", {VALUE, REGISTER}},
    };

    const set<string> NIBBLE_INSTRUCTIONS = {"LDI", "STI", "BIU", "BIG", "BIL", "BIE", "BIN", "SET"};
    const set<string> NO_OPERAND_INSTRUCTIONS = {"NOP", "HLT"};
    const set<string> SINGLE_REGISTER_INSTRUCTIONS = {"INC", "DEC", "MTD", "MFD", "TST"};
    const set<string> DOUBLE_REGISTER_INSTRUCTIONS = {"ADD", "ADC", "SUB", "SBC", "INV", "XOR", "AND", "ORR", "NOT", "LDD", "STD", "BDU", "BDG", "BDL", "BDE", "BDN", "MOV", "CMP"};

    auto AssembleToDenary(const string &file_, const int line_, const string &instruction) -> pair<int, int>;
    auto AssembleToString(const string &file_, const int line_, const string &instruction) -> string;
    auto AssembleLinesToDenary(const string &file_, const vector<string> lines) -> vector<int>;
    auto AssembleLinesToDenaryPairs(const string &file_, const vector<string> lines) -> vector<pair<int, int>>;
}