#include "ComponentTestPrint.hpp"

#include <Util/Colors.hpp>
#include <thread>



namespace ComponentTestPrint {
    namespace {
        auto GetCommand() -> char {
            string command;
            while ((command != "p") && (command != "n") && (command != "c") && (command != "e") && (command != "s")) {
                std::cout << YELLOW << "[ " << WHITE << "p" << NO_COLOR << " = previous"
                        << YELLOW << " | " << WHITE << "n" << NO_COLOR << " = next"
                        << YELLOW << " | " << WHITE << "s" << NO_COLOR << " = skip"
                        << YELLOW << " | " << WHITE << "c" << NO_COLOR << " = continue"
                        << YELLOW << " | " << WHITE << "e" << NO_COLOR << " = exit"
                        << YELLOW << " ]" << NO_COLOR << "\n";
                std::cin >> command;
            }
            return command.at(0);
        }

        auto PrintInputs(const ComponentTestCase &test_case) -> void {
            std::cout << WHITE << "Inputs" << "\n";
            for (const auto &input_pair : test_case.input) {
                std::cout << NO_COLOR << "- " << input_pair.first << ": " << CYAN << input_pair.second.value << "\n";
            }
        }

        auto SetInputs(const unordered_map<string, GPIO::PinBlock> &pin_blocks, const ComponentTestCase &test_case) -> void {
            for (const auto &input_pair : test_case.input) {
                const GPIO::PinBlock &pin_block = pin_blocks.at(input_pair.first);
                GPIO::Set(pin_block, input_pair.second);
            }
        }

        auto PrintOutputs(const unordered_map<string, GPIO::PinBlock> &pin_blocks, const ComponentTestCase &test_case) -> void {
            std::cout << WHITE << "Outputs" << "\n";
            if (test_case.expected_output.empty()) {
                std::cout << NO_COLOR << "- N/A" << "\n";
            }
            for (const auto &expected_output_pair : test_case.expected_output) {
                const GPIO::PinBlock &pin_block = pin_blocks.at(expected_output_pair.first);
                const int expected = expected_output_pair.second.value;
                const int actual = GPIO::ReadInt(pin_block, expected_output_pair.second.is_signed);
                if (actual == expected) {
                    std::cout << NO_COLOR << "- " << expected_output_pair.first << ": " << GREEN << expected << WHITE << " | " << GREEN << actual << "\n";
                } else {
                    std::cout << NO_COLOR << "- " << expected_output_pair.first << ": " << GREEN << expected << WHITE << " | " << RED << actual << "\n";
                }
            }
        }

        auto GetSkipCount() -> int {
            cout << WHITE << "Number of tests to skip" << CYAN << "\n";
            string test_string;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> test_string;
            return std::stoi(test_string);
        }

        auto SkipTests(const unordered_map<string, GPIO::PinBlock> &pin_blocks, const vector<ComponentTestCase> &tests, const int test_count, int &i) -> void {
            for (int j = 0; j < test_count; j++) {
                ComponentTestCase new_test_case = tests.at(i);

                for (const auto &input_pair : new_test_case.input) {
                    const GPIO::PinBlock &pin_block = pin_blocks.at(input_pair.first);
                    GPIO::Set(pin_block, input_pair.second);
                    std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));
                }

                i++;
            }
        }
    }
    
    auto LoopTestResults(const vector<ComponentTestCase> &tests, const unordered_map<string, GPIO::PinBlock> &pin_blocks) -> void { //NOLINT(readability-convert-member-functions-to-static)
        bool require_test_failed = true;
        for (int i = 0; i < tests.size(); i++) {
            ComponentTestCase test_case = tests.at(i);

            if (require_test_failed && test_case.passed) {
                continue;
            }

            std::cout << RED << "--- TEST CASE " << i << " ---" << "\n";

            PrintInputs(test_case);
            SetInputs(pin_blocks, test_case);
            PrintOutputs(pin_blocks, test_case);

            char command = GetCommand();

            if (command == 'p') {
                if (i <= 0) {
                    cout << RED << "No previous test; staying on current test" << "\n";
                    i -= 1;
                    continue;
                }
                require_test_failed = false;
                i-=2;
            }

            else if (command == 'n') {
                if (i >= tests.size()-1) {
                    cout << RED << "No next test; staying on current test" << "\n";
                    continue;
                }
                require_test_failed = false;
            }

            else if (command == 's') {
                const int test_count = GetSkipCount();
                if ((test_count < 0) || (test_count > tests.size())) {
                    cout << RED << "Index out of bounds" << WHITE << "\n";
                    continue;
                }

                std::cout << test_count;
                SkipTests(pin_blocks, tests, test_count, i);
                std::cout << WHITE;
            }

            else if (command == 'c') {
                require_test_failed = true;
            }

            else if (command == 'e') {
                break;
            }

            std::cout << "\n";
        }
    }
}