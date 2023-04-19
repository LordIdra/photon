#include "SimulationTestBuilder.hpp"
#include "Assembler/Assembler.hpp"
#include "Util/Errors.hpp"
#include <string>



namespace SimulationTestBuilder {
    namespace {
        const string HEADER = "Tick Pulse Test_out";

        string file;
        int line;

        auto Assemble(const string &text) -> pair<int, int> {
            pair<int, int> instruction;
            try {
                instruction = Assembler::AssembleToDenary(file, line, text);
            } catch (const std::runtime_error &e) {
                std::cerr << e.what();
                return make_pair(0, 0);
            }
            return instruction;
        }

        auto AddInstructionMemory(vector<string> &memory, const pair<int, int> instruction, const string &text, int &memory_index) -> void {
            memory.push_back("# " + text);
            memory.push_back("memory ProgramMemory(" + to_string(memory_index) + ")=" + to_string(instruction.first) + ";");
            memory_index++;
            memory.push_back("memory ProgramMemory(" + to_string(memory_index) + ")=" + to_string(instruction.second) + ";");
            memory_index++;
        }

        auto AddInstructionOperations(vector<string> &operations, const string &text) -> void {
            operations.push_back("# " + text);
            for (int i = 0; i < 8; i++) {
                operations.emplace_back(to_string(i+1) + " 1 X");
                operations.emplace_back(to_string(i+1) + " 0 X");
            }
        }

        auto ProcessLine(int &memory_index, const string &text, vector<string> &memory, vector<string> &operations) -> void {
            // Nothing
            if (text.empty()) {
                return;
            }

            // Expect
            if (text.at(0) == '!') {
                if (operations.empty()) {
                    throw Errors::TestBuilder::NothingToExpect(file, line);
                }
                string expected = text.substr(1, text.size()-1);
                string &operation = operations.at(operations.size()-1);
                operation.erase(operation.size()-1);
                operation += "(" + expected + ")";
                return;
            }

            // Instruction that won't be executed (eg if it is being branched over)
            if (text.at(0) == '~') {
                string instruction_text = text.substr(1, text.size()-1);
                pair<int, int> instruction = Assemble(instruction_text);
                AddInstructionMemory(memory, instruction, instruction_text, memory_index);
                return;
            }

            // Instruction
            pair<int, int> instruction = Assemble(text);
            AddInstructionMemory(memory, instruction, text, memory_index);
            AddInstructionOperations(operations, text);
        }
    }

    auto Build(const string &file_, const vector<string> &input) -> vector<string> {
        file = file_;
        vector<string> memory;
        vector<string> operations;

        int memory_index = 0;

        for (int i = 0; i < input.size(); i++) {
            line = i+1;
            ProcessLine(memory_index, input.at(i), memory, operations);
        }

        vector<string> output;
        output.push_back(HEADER);
        for (const string &text : memory)     { output.push_back(text); }
        for (const string &text : operations) { output.push_back(text); }

        return output;
    }
}