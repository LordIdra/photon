#include "Assembler.hpp"
#include "Util/Errors.hpp"

#include <Util/Util.hpp>

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>



namespace Assembler {

    namespace {

        int line;
        string file;
        
        // https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
        auto Split(const string &input, vector<string> &output, char delimiter) -> void{
            size_t position = input.find(delimiter);
            size_t initial_position = 0;

            while (position != std::string::npos) {
                output.push_back(input.substr(initial_position, position - initial_position));
                initial_position = position + 1;
                position = input.find(delimiter, initial_position);
            }

            output.push_back(input.substr(initial_position, std::min( position, input.size()) - initial_position + 1));
        }

        auto IsValidOpcode(const string &opcode_string) -> bool {
            return OPCODES.find(opcode_string) != OPCODES.end();
        }

        auto ExtractOperand(string operand) -> pair<OperandType, int> {
            OperandType type = REGISTER;
            int value = 0;

            if (operand.at(0) == 'r') {
                type = REGISTER;
                operand.erase(0, 1);
            } else {
                type = VALUE;
            }

            try {
                value = std::stoi(operand);
            } catch (std::exception &e) {
                throw Errors::Assembler::InvalidOperandValue(file, line, operand);
            }

            return make_pair(type, value);
        }

        auto ExtractOperands(const string &opcode_string, const vector<string> &operand_strings) -> vector<int>{
            vector<int> operands;
            for (int i = 0; i < operand_strings.size(); i++) {
                pair<OperandType, int> operand = ExtractOperand(operand_strings.at(i));

                // Check that actual and expected operand type match
                if (operand.first != OPERANDS.at(opcode_string).at(i)) {
                    throw Errors::Assembler::InvalidOperandType(file, line, operand_strings.at(i));
                }

                operands.push_back(operand.second);
            }

            return operands;
        }

        auto HandleBoundValue(int &operand) -> void {
            if ((operand < MIN_VALUE) || (operand > MAX_VALUE)) {
                throw Errors::Assembler::OperandOutOfBounds(file, line, operand);
            }
            if (operand < 0) {
                operand += 256;
            }
        }

        auto HandleBoundRegister(int &operand) -> void {
            if ((operand < MIN_REGISTER) || (operand > MAX_REGISTER)) {
                throw Errors::Assembler::OperandOutOfBounds(file, line, operand);
            }
        }

        auto HandleBoundAddress(int &operand) -> void {
            if ((operand < MIN_ADDRESS) || (operand > MAX_ADDRESS)) {
                throw Errors::Assembler::OperandOutOfBounds(file, line, operand);
            }
        }

        auto HandleBounds(const string &opcode, vector<int> &operands) -> void {
            if (opcode == "SET") {
                HandleBoundValue(operands.at(0));
                HandleBoundRegister(operands.at(1));
            }

            else if (NIBBLE_INSTRUCTIONS.count(opcode) == 1) {
                HandleBoundAddress(operands.at(0));
            }

            else if (SINGLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                HandleBoundRegister(operands.at(0));
            }

            else if (DOUBLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                HandleBoundRegister(operands.at(0));
                HandleBoundRegister(operands.at(1));
            }
        }

        auto GetOperands(const string &opcode_string, const string &operand_string) -> vector<int> {
            vector<string> operand_strings;
            Split(operand_string, operand_strings, ' ');
            
            // Check actual and expected operand count match
            const int expected_operand_count = OPERANDS.at(opcode_string).size();
            if (operand_strings.size() != expected_operand_count) {
                throw Errors::Assembler::InvalidOperandCount(file, line, operand_strings.size(), expected_operand_count);
            }

            // Convert string operands to int operands
            vector<int> operands = ExtractOperands(opcode_string, operand_strings);

            // Check that any values are properly bounded and convert any negative numbers to equivalent unsigned values
            HandleBounds(opcode_string, operands);

            return operands;
        }

        auto OperandsAsIntegers(const vector<string> &operand_strings) -> vector<int> {
            vector<int> operand_ints;
            operand_ints.reserve(operand_strings.size());
            for (const string &operand_string : operand_strings) {
                operand_ints.push_back(std::stoi(operand_string));
            }
            return operand_ints;
        }

        auto OperandsAsBinary(const vector<int> &operand_ints) -> vector<string> {
            vector<string> operand_binary;
            operand_binary.reserve(operand_ints.size());
            for (const int &operand_int : operand_ints) {
                operand_binary.push_back(DenaryToBinaryString(operand_int, WORD_SIZE));
            }
            return operand_binary;
        }

        auto Combine(const string &opcode, const vector<int> &operands) -> pair<int, int> {
            int first_byte = 0;
            int second_byte = 0;

            first_byte += OPCODES.at(opcode);
            
            if (opcode == "SET") {
                first_byte += div(operands.at(0), 16).quot;
                second_byte += div(operands.at(0), 16).rem*16;
                second_byte += operands.at(1);
            }

            else if (NIBBLE_INSTRUCTIONS.count(opcode) == 1) {
                first_byte += div(operands.at(0), 256).quot;
                second_byte += div(operands.at(0), 256).rem;
            }

            else if (SINGLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                second_byte += operands.at(0);
            }

            else if (DOUBLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                second_byte += operands.at(0);
                second_byte += 16*operands.at(1);
            }

            return make_pair(first_byte, second_byte);
        }
    }

    auto AssembleToDenary(const string &file_, const int line_, const string &instruction) -> pair<int, int> {
        file = file_;
        line = line_;

        // Get opcode string and check opcode is valid
        string opcode_string = instruction.substr(0, 3);
        if (!IsValidOpcode(opcode_string)) {
            throw Errors::Assembler::InvalidOpcode(file, line, opcode_string);
        }

        // Get operands if there are any
        vector<int> operands;
        if (instruction.size() > 3) {
            string operand_string = instruction.substr(4, instruction.size() - 4);
            operands = GetOperands(opcode_string, operand_string);
        } 
        
        else {
            // Check that the instruction should not have any operands
            if (!OPERANDS.at(opcode_string).empty()) {
                throw Errors::Assembler::InvalidOperandCount(file, line, 0, OPERANDS.at(opcode_string).size());
            }
        }

        return Combine(opcode_string, operands);
    }

    auto AssembleToString(const string &file_, const int line_, const string &instruction) -> string {
        file = file_;
        line = line_;
        pair<int, int> bytes = AssembleToDenary(file_, line, instruction);
        return to_string(bytes.first) + " " + to_string(bytes.second);
    }
}