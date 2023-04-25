#include "ComponentTest.hpp"

#include "../Util/Colors.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include "Util/Errors.hpp"

#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>



auto ComponentTest::Init() -> void {
    vector<GPIO::PinBlock> blocks;
    for (const auto &block_pair : pin_blocks) {
        blocks.push_back(block_pair.second);
    }
    GPIO::Setup(blocks);
}

auto ComponentTest::LoopTestResults(const vector<ComponentTestCase> &tests) -> void { //NOLINT(readability-convert-member-functions-to-static)
    bool require_test_failed = true;
    for (int i = 0; i < tests.size(); i++) {
        ComponentTestCase test_case = tests.at(i);
        if (require_test_failed && test_case.passed) {
            continue;
        }

        std::cout << RED << "--- TEST CASE " << i << " ---" << "\n";

        std::cout << WHITE << "Inputs" << "\n";
        for (const auto &input_pair : test_case.input) {
            std::cout << NO_COLOR << "- " << input_pair.first << ": " << CYAN << input_pair.second << "\n";
        }
        
        std::cout << WHITE << "Outputs" << "\n";
        if (test_case.expected_output.empty()) {
            std::cout << NO_COLOR << "- N/A" << "\n";
        }
        for (const auto &input_pair : test_case.input) {
            const GPIO::PinBlock &pin_block = pin_blocks.at(input_pair.first);
            GPIO::Set(pin_block, input_pair.second);
        }
        for (const auto &expected_output_pair : test_case.expected_output) {
            const GPIO::PinBlock &pin_block = pin_blocks.at(expected_output_pair.first);
            const int expected = expected_output_pair.second;
            const int actual = GPIO::ReadInt(pin_block);
            if (actual == expected) {
                std::cout << NO_COLOR << "- " << expected_output_pair.first << ": " << GREEN << expected << WHITE << " | " << GREEN << actual << "\n";
            } else {
                std::cout << NO_COLOR << "- " << expected_output_pair.first << ": " << GREEN << expected << WHITE << " | " << RED << actual << "\n";
            }
        }

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

        if (command == "p") {
            if (i <= 0) {
                cout << RED << "No previous test; staying on current test" << "\n";
                i -= 1;
                continue;
            }
            require_test_failed = false;
            i-=2;
        }

        else if (command == "n") {
            if (i >= tests.size()-1) {
                cout << RED << "No next test; staying on current test" << "\n";
                continue;
            }
            require_test_failed = false;
        }

        else if (command == "s") {
            cout << WHITE << "Number of tests to skip" << CYAN << "\n";
            string test_string;
            getline(std::cin, test_string, '\n');
            try {
                const int test_count = std::stoi(test_string);
                if ((test_count < 0) || (test_count > tests.size())) {
                    cout << RED << "Index out of bounds" << WHITE << "\n";
                    continue;
                }
                std::cout << test_count;
                //for (int i = 0; i < tests.size(); i++) {
            } catch (const std::invalid_argument &e) {
                cout << RED << "Invalid input" << "WHITE" << "\n";
            }
            std::cout << WHITE;
        }

        else if (command == "c") {
            require_test_failed = true;
        }

        else if (command == "e") {
            break;
        }

        std::cout << "\n";
    }
}

auto ComponentTest::PrintTestResults() -> void {
    unordered_map<string, int> passed_tests_for_group;
    unordered_map<string, int> total_tests_for_group;

    for (const auto &test_pair : test_groups) {
        const int total_tests = test_pair.second.size();
        int passed_tests = 0;
        for (const ComponentTestCase &test_case : test_pair.second) {
            if (test_case.passed) {
                passed_tests++;
            }
        }
        passed_tests_for_group.insert(make_pair(test_pair.first, passed_tests));
        total_tests_for_group.insert(make_pair(test_pair.first, total_tests));
    }

    for (const auto &test_pair : test_groups) {
        cout << YELLOW << test_pair.first << 
            WHITE << " - " << CYAN << passed_tests_for_group.at(test_pair.first) << WHITE << "/" << CYAN << total_tests_for_group.at(test_pair.first) << "\n";
    }

    std::cout << "\n";

    for (const auto &test_pair : test_groups) {
        if (passed_tests_for_group.at(test_pair.first) != total_tests_for_group.at(test_pair.first)) {
            cout << WHITE << "[[[ Looping test results for " << YELLOW << test_pair.first << WHITE << " ]]]" "\n";
            LoopTestResults(test_pair.second);
        }
    }
}

auto ComponentTest::Run() -> void {
    Init();
    timer.Start();
    RunTestGroups();
    timer.Stop();
    cout << WHITE << "[[[ Test complete in " << CYAN << timer.GetMilliseconds() << "ms" << WHITE << " ]]]" << NO_COLOR << "\n";
    PrintTestResults();
}

auto ComponentTest::RunTestCase(ComponentTestCase &test_case) -> void {
    for (const auto &input_pair : test_case.input) {
        const GPIO::PinBlock &pin_block = pin_blocks.at(input_pair.first);
        GPIO::Set(pin_block, input_pair.second);
    }

    std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));

    for (const auto &expected_output_pair : test_case.expected_output) {
        const GPIO::PinBlock &pin_block = pin_blocks.at(expected_output_pair.first);
        const int expected = expected_output_pair.second;
        const int actual = GPIO::ReadInt(pin_block);
        test_case.passed = expected == actual;
    }
}

auto ComponentTest::RunTestGroup(const string &name, vector<ComponentTestCase> &test_group) -> void {
    cout << WHITE << "[ Running group " << YELLOW << name << WHITE << " ]" << "\n";
    int i = 1;
    for (ComponentTestCase &test_case : test_group) {
        RunTestCase(test_case);
        cout << WHITE << i << CYAN << "/" << WHITE << test_group.size() << "\n";
        i++;
    }
}

auto ComponentTest::RunTestGroups() -> void {
    for (auto &test_group_pair : test_groups) {
        RunTestGroup(test_group_pair.first, test_group_pair.second);
    }
}
