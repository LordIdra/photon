#include "ComponentTest.hpp"

#include "../Util/Colors.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "ComponentTestPrint.hpp"
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

auto ComponentTest::GetPassedTests() -> unordered_map<string, int> {
    unordered_map<string, int> passed_tests_for_group;
    for (const auto &test_pair : test_groups) {
        int passed_tests = 0;
        for (const ComponentTestCase &test_case : test_pair.second) {
            if (test_case.passed) {
                passed_tests++;
            }
        }
        passed_tests_for_group.insert(make_pair(test_pair.first, passed_tests));
    }
    return passed_tests_for_group;
}

auto ComponentTest::GetTotalTests() -> unordered_map<string, int> {
    unordered_map<string, int> total_tests_for_group;
    for (const auto &test_pair : test_groups) {
        const int total_tests = test_pair.second.size();
        total_tests_for_group.insert(make_pair(test_pair.first, total_tests));
    }
    return total_tests_for_group;
}

auto ComponentTest::PrintGroupResults(const unordered_map<string, int> &passed_tests_for_group, const unordered_map<string, int> &total_tests_for_group) -> void {
    for (const auto &test_pair : test_groups) {
        cout << YELLOW << test_pair.first << 
            WHITE << " - " << CYAN << passed_tests_for_group.at(test_pair.first) << WHITE << "/" << CYAN << total_tests_for_group.at(test_pair.first) << "\n";
    }
    std::cout << "\n";

}

auto ComponentTest::PrintTestResults() -> void {
    cout << WHITE << "[[[ Test complete in " << CYAN << timer.GetMilliseconds() << "ms" << WHITE << " ]]]" << NO_COLOR << "\n";

    unordered_map<string, int> passed_tests_for_group = GetPassedTests();
    unordered_map<string, int> total_tests_for_group = GetTotalTests();

    PrintGroupResults(passed_tests_for_group, total_tests_for_group);
    
    for (const auto &test_pair : test_groups) {
        const bool all_tests_passed = passed_tests_for_group.at(test_pair.first) == total_tests_for_group.at(test_pair.first);
        if (!all_tests_passed) {
            cout << WHITE << "[[[ Looping test results for " << YELLOW << test_pair.first << WHITE << " ]]]" "\n";
            ComponentTestPrint::LoopTestResults(test_pair.second, pin_blocks);
        }
    }
}

auto ComponentTest::Run() -> void {
    Init();
    timer.Start();
    RunTestGroups();
    timer.Stop();
    PrintTestResults();
}

auto ComponentTest::RunTestCase(ComponentTestCase &test_case) -> void {
    for (const auto &input_pair : test_case.input) {
        const GPIO::PinBlock &pin_block = pin_blocks.at(input_pair.first);
        GPIO::Set(pin_block, input_pair.second);
    }

    std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));

    test_case.passed = true;

    for (const auto &expected_output_pair : test_case.expected_output) {
        const GPIO::PinBlock &pin_block = pin_blocks.at(expected_output_pair.first);
        const int expected = expected_output_pair.second.value;
        const int actual = GPIO::ReadInt(pin_block, expected_output_pair.second.is_signed);
        test_case.actual_output.insert(std::make_pair(expected_output_pair.first, TestNumber{actual, expected_output_pair.second.is_signed}));
        if (expected != actual) {
            test_case.passed = false;
        }
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
