#include "Assembler.hpp"
#include "Util/Errors.hpp"

#include <Util/Util.hpp>

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>



namespace Assembler {

    namespace {
        
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

        auto ExtractOperand(const int line, const string &instruction, string operand) -> pair<OperandType, int> {
            OperandType type;
            int value;

            if (operand.at(0) == 'r') {
                type = REGISTER;
                operand.erase(0);
            } else {
                type = VALUE;
            }

            try {
                value = atoi(operand.c_str());
            } catch (std::exception e) {
                Errors::Asssembler::InvalidOperand(line, operand);
                value = 0;
            }

            return make_pair(type, value);
        }

        auto ExtractOperands(int line, const string &instruction, const string &opcode_string, const vector<string> &operand_strings) -> vector<int>{
            vector<int> operands;
            for (int i = 0; i < operand_strings.size(); i++) {
                pair<OperandType, int> operand = ExtractOperand(line, instruction, operand_strings.at(i));

                // Check that operand actual dna expected operand type match
                if (operand.first != OPERANDS.at(opcode_string).at(i)) {
                    Errors::Asssembler::InvalidOperand(line, operand_strings.at(i));
                }

                operands.push_back(operand.second);
            }

            return operands;
        }

        auto HandleBoundValue(int line, int &operand) -> void {
            if ((operand < MIN_VALUE) || (operand > MAX_VALUE)) {
                Errors::Asssembler::OperandOutOfBounds(line, operand);
            }
            if (operand < 0) {
                operand += 256;
            }
        }

        auto HandleBoundRegister(int line, int &operand) -> void {
            if ((operand < MIN_REGISTER) || (operand > MAX_REGISTER)) {
                Errors::Asssembler::OperandOutOfBounds(line, operand);
            }
        }

        auto HandleBoundAddress(int line, int &operand) -> void {
            if ((operand < MIN_ADDRESS) || (operand > MAX_ADDRESS)) {
                Errors::Asssembler::OperandOutOfBounds(line, operand);
            }
        }

        auto HandleBounds(int line, const string &opcode, vector<int> &operands) -> void {
            if (opcode == "SET") {
                HandleBoundValue(line, operands.at(0));
                HandleBoundRegister(line, operands.at(1));
            }

            else if (NIBBLE_INSTRUCTIONS.count(opcode) == 1) {
                HandleBoundAddress(line, operands.at(0));
            }

            else if (SINGLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                HandleBoundRegister(line, operands.at(0));
            }

            else if (DOUBLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                HandleBoundRegister(line, operands.at(0));
                HandleBoundRegister(line, operands.at(1));
            }
        }

        auto GetOperands(const int line, const string &instruction, const string &opcode_string, const string &operand_string) -> vector<int> {
            vector<string> operand_strings;
            Split(operand_string, operand_strings, ' ');
            
            // Check actual and expected operand count match
            const int expected_operand_count = OPERANDS.at(opcode_string).size();
            if (operand_strings.size() != expected_operand_count) {
                Errors::Asssembler::InvalidOperandCount(line, operand_strings.size(), expected_operand_count);
            }

            // Convert string operands to int operands
            vector<int> operands = ExtractOperands(line, instruction, opcode_string, operand_strings);

            // Check that any values are properly bounded and convert any negative numbers to equivalent unsigned values
            HandleBounds(line, opcode_string, operands);

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
                operand_binary.push_back(DenaryToBinary(operand_int, WORD_SIZE));
            }
            return operand_binary;
        }

        auto Combine(const string &opcode, const vector<int> operands) -> pair<int, int> {
            int first_byte = 0;
            int second_byte = 0;

            first_byte += OPCODES.at(opcode);
            
            if (opcode == "SET") {
                first_byte += div(operands.at(0), 16).quot;
            }

            else if (NIBBLE_INSTRUCTIONS.count(opcode) == 1) {
                first_byte += div(operands.at(0), 256).quot;
                second_byte += div(operands.at(0), 256).rem;
            }

            else if (SINGLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                second_byte += operands.at(0);
            }

            else if (DOUBLE_REGISTER_INSTRUCTIONS.count(opcode) == 1) {
                second_byte += operands.at(1);
                second_byte += 16*operands.at(0);
            }

            return make_pair(first_byte, second_byte);
        }
    }

    auto Assemble(const int line, const string &instruction) -> string {
        // Get opcode string and check opcode is valid
        string opcode_string = instruction.substr(0, 3);
        if (!IsValidOpcode(opcode_string)) {
            Errors::Asssembler::InvalidOpcode(line, opcode_string);
            return "";
        }

        // Get operands
        string operand_string = instruction.substr(3, instruction.size() - 3);
        vector<int> operands = GetOperands(line, instruction, opcode_string, operand_string);

        pair<int, int> bytes = Combine(opcode_string, operands);

        return to_string(bytes.first) + " " + to_string(bytes.second);
    }
    
}