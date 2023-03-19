#include "CommandLine.hpp"
#include "Files/Files.hpp"
#include "TestBuilder/TestBuilder.hpp"



namespace CommandLine {
    namespace {
        const string INPUT_FILES = "resources/tests/in/";
        const string OUTPUT_FILES = "resources/tests/out/";
        auto DisplayHelpMessage() -> void {
            std::cout << "Photon Subcommands" << "\n";
            std::cout << "    build-tests: Compiles all the tests in resources/tests/inputs and outputs them in resources/tests/outputs" << "\n";
        }
    }

    auto Command(const vector<string> &arguments) -> void {
        if (arguments.empty()) {
            DisplayHelpMessage();
            return;
        }

        if (arguments.at(0) == "build-tests") {
            string filename = std::filesystem::current_path();
            vector<string> filenames = Files::FilePaths(INPUT_FILES);
            
            for (const string &filename : filenames) {
                const string input_file = INPUT_FILES + filename;
                const string output_file = OUTPUT_FILES + filename;

                vector<string> input = Files::Read(input_file);
                vector<string> output = TestBuilder::Build(filename, input);

                Files::Write(output_file, output);
            }
            
            return;
        }

        DisplayHelpMessage();
    }
}