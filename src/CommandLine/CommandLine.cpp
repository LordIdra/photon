#include "CommandLine.hpp"

#include "Assembler/Assembler.hpp"
#include "ComponentTest/ComponentTests.hpp"
#include "ComponentTest/IntegrationTests.hpp"
#include "Files/Files.hpp"
#include "SimulationTestBuilder/SimulationTestBuilder.hpp"
#include "Electronics/ProgramMemory.hpp"
#include "Electronics/Debug.hpp"
#include "Util/Errors.hpp"



namespace CommandLine {
    namespace {
        const string TEST_INPUT_FILES = "resources/tests/in/";
        const string TEST_OUTPUT_FILES = "resources/tests/out/";
        const string PROGRAM_FILES = "resources/programs/";

        auto DisplayHelpMessage() -> void {
            std::cout << "Photon Subcommands" << "\n";
            std::cout << "    build-simulation-tests: Compiles all the simulation tests in resources/tests/inputs and outputs them in resources/tests/outputs" << "\n";
            std::cout << "    run-component-test [name]: Attempts to run the specified component test using GPIO pins" << "\n";
            std::cout << "    run-integration-test [name]: Attempts to run the specified integration test using GPIO pins" << "\n";
            std::cout << "    reset-integration-pins [name]: Reset the integration test pins" << "\n";
            std::cout << "    test-eeprom: Verifies the integrity of the EEPROM" << "\n";
            std::cout << "    write-program [file]: Assembles the specified file and writes it to a connected EEPROM" << "\n";
            std::cout << "    debug: Allows sending clock pulses to the last GPIO pin for debugging" << "\n";
        }

        auto BuildSimulationTests() -> void {
            string filename = std::filesystem::current_path();
            vector<string> filenames = Files::FilePaths(TEST_INPUT_FILES);

            for (const string &filename : filenames) {
                const string input_file = TEST_INPUT_FILES + filename;
                const string output_file = TEST_OUTPUT_FILES + filename;

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
            else if (name == "RegisterFile")  { ComponentTests::TestRegisterFile(); } 
            else if (name == "ALU-OutFlags")  { ComponentTests::TestALU_OutFlags(); } 
            else if (name == "ALU-OutResult") { ComponentTests::TestALU_OutResult(); } 
            else if (name == "ALU-OutCarry")  { ComponentTests::TestALU_OutCarry(); } 
            else if (name == "ALU-OutOr")     { ComponentTests::TestALU_OutOr(); } 
            else { std::cout << "Invalid component; available components are [MemAdderAndRPC, RegisterFile, ALU-OutFlags, ALU-OutResult, ALU-OutCarry, ALU-OutOr]" << "\n"; }
        }

        auto RunIntegrationTest(const vector<string> &arguments) {
            if (arguments.size() != 2) {
                std::cout << "Incorrect number of arguments" << "\n";
                return;
            }

            const string name = PROGRAM_FILES + arguments.at(1);
            const vector<string> lines = Files::Read(name);
            const vector<pair<int, int>> assembly = Assembler::AssembleLinesToDenaryPairs(name, lines);

            IntegrationTests::RunInstructions(assembly);
        }

        auto TestEEPROM() -> void {
            ProgramMemory::Test();
        }

        auto WriteProgram(const vector<string> &arguments) -> void {
            if (arguments.size() != 2) {
                std::cout << "Incorrect number of arguments" << "\n";
                return;
            }

            const string name = PROGRAM_FILES + arguments.at(1);
            const vector<string> lines = Files::Read(name);
            const vector<int> assembly = Assembler::AssembleLinesToDenary(name, lines);

            if (assembly.size() > 4096) {
                throw Errors::ProgramMemory::ProgramTooLarge(assembly.size());
            }

            ProgramMemory::Write(assembly);
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

        if (arguments.at(0) == "run-integration-test") {
            RunIntegrationTest(arguments);
            return;
        }

        if (arguments.at(0) == "reset-integration-pins") {
            IntegrationTests::ResetTestPins();
            return;
        }

        if (arguments.at(0) == "test-eeprom") {
            TestEEPROM();
            return;
        }

        if (arguments.at(0) == "write-program") {
            WriteProgram(arguments);
            return;
        }

        if (arguments.at(0) == "debug") {
            Debug::DebugLoop();
            return;
        }

        DisplayHelpMessage();
    }
}