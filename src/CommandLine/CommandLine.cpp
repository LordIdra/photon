#include "CommandLine.hpp"

#include "ComponentTest/ComponentTests.hpp"
#include "Files/Files.hpp"
#include "SimulationTestBuilder/SimulationTestBuilder.hpp"
#include "ProgramMemory/ProgramMemory.hpp"



namespace CommandLine {
    namespace {
        const string INPUT_FILES = "resources/tests/in/";
        const string OUTPUT_FILES = "resources/tests/out/";

        auto DisplayHelpMessage() -> void {
            std::cout << "Photon Subcommands" << "\n";
            std::cout << "    build-simulation-tests: Compiles all the simulation tests in resources/tests/inputs and outputs them in resources/tests/outputs" << "\n";
            std::cout << "    run-component-test [name]: Attempts to run the specified component test using GPIO pins" << "\n";
            std::cout << "    test-eeprom: Verifies the integrity of the EEPROM" << "\n";
            std::cout << "    write-program [file]: Assembles the specified file and writes it to a connected EEPROM" << "\n";
        }

        auto BuildSimulationTests() -> void {
            string filename = std::filesystem::current_path();
            vector<string> filenames = Files::FilePaths(INPUT_FILES);

            for (const string &filename : filenames) {
                const string input_file = INPUT_FILES + filename;
                const string output_file = OUTPUT_FILES + filename;

                vector<string> input = Files::Read(input_file);
                vector<string> output = SimulationTestBuilder::Build(filename, input);

                Files::Write(output_file, output);
            }
        }

        auto RunComponentTest(const vector<string> &arguments) -> void {
            if (arguments.size() != 2) {
                std::cout << "Incorrect number of arguments" << "\n";
                return;
            }

            const string name = arguments.at(1);

            if      (name == "MemAdderAndRPC"){ ComponentTests::TestMemAdderAndRPC(); } 
            else if (name == "SCU")           { ComponentTests::TestSCU(); } 
            else if (name == "RegisterFile")  { ComponentTests::TestRegisterFile(); } 
            else if (name == "ALU-OutFlags")  { ComponentTests::TestALU_OutFlags(); } 
            else if (name == "ALU-OutResult") { ComponentTests::TestALU_OutResult(); } 
            else if (name == "ALU-OutCarry")  { ComponentTests::TestALU_OutCarry(); } 
            else if (name == "ALU-OutOr")     { ComponentTests::TestALU_OutOr(); } 
            else { std::cout << "Invalid component; available components are [MemAdderAndRPC, SCU, RegisterFile, ALU-OutFlags, ALU-OutResult, ALU-OutCarry, ALU-OutOr]" << "\n"; }
        }

        auto TestEEPROM(const int WRITE_DELAY_MICROSECONDS, const int READ_DELAY_MICROSECONDS) -> void {
            ProgramMemory::Test(WRITE_DELAY_MICROSECONDS, READ_DELAY_MICROSECONDS);
        }
    }

    auto Command(const vector<string> &arguments) -> void {
        if (arguments.empty()) {
            DisplayHelpMessage();
            return;
        }

        if (arguments.at(0) == "build-simulation-tests") {
            BuildSimulationTests();
            return;
        }

        if (arguments.at(0) == "run-component-test") {
            RunComponentTest(arguments);
            return;
        }

        if (arguments.at(0) == "test-eeprom") {
            TestEEPROM(std::stoi(arguments.at(1)), std::stoi(arguments.at(2)));
            return;
        }

        DisplayHelpMessage();
    }
}