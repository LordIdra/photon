#include "CommandLine.hpp"

#include "ComponentTest/ComponentTests.hpp"
#include "Files/Files.hpp"
#include "SimulationTestBuilder/SimulationTestBuilder.hpp"



namespace CommandLine {
    namespace {
        const string INPUT_FILES = "resources/tests/in/";
        const string OUTPUT_FILES = "resources/tests/out/";

        auto DisplayHelpMessage() -> void {
            std::cout << "Photon Subcommands" << "\n";
            std::cout << "    build-simulation-tests: Compiles all the simulation tests in resources/tests/inputs and outputs them in resources/tests/outputs" << "\n";
            std::cout << "    run-component-test [name]: Attempts to run the specified component test using GPIO pins" << "\n";
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

            if      (name == "MemAdder")      { ComponentTests::TestMemAdder(); } 
            else if (name == "SCU")           { ComponentTests::TestSCU(); } 
            else if (name == "RegisterFile")  { ComponentTests::TestRegisterFile(); } 
            else if (name == "ALU-OutFlags")  { ComponentTests::TestALU_OutFlags(); } 
            else if (name == "ALU-OutResult") { ComponentTests::TestALU_OutResult(); } 
            else if (name == "ALU-OutCarry")  { ComponentTests::TestALU_OutCarry(); } 
            else if (name == "ALU-OutOr")     { ComponentTests::TestALU_OutOr(); } 
            else { std::cout << "Invalid component; available components are [MemAdder, SCU, RegisterFile, ALU-OutFlags, ALU-OutResult, ALU-OutCarry, ALU-OutOr]" << "\n"; }
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

        DisplayHelpMessage();
    }
}