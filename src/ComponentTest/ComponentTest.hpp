#pragma once

#include "ComponentTest/ComponentTestCase.hpp"

#include "../Util/Types.hpp"
#include "../Util/Timer.hpp"
#include "Electronics/GPIO.hpp"
#include <unordered_map>



/* 
    [ Component Test Configurations ]
    Clock             | N/A
    Decoder16         | N/A
    OperandIntegrator | N/A
    Decoder8          | N/A
    ProgramMemory     | N/A
    RAM               | N/A
    RegisterBuffer    | N/A
    Addressor         | N/A
    RPC               | N/A
    MemAdder          | ->12 <-12
    SCU               | ->1 <-25 (requires connected ProgramMemory, RPC, MemAdder, and Decoder8)
    RegisterFile      | ->1 ->4 ->8 <-8 (requires connected Decoder16)
    ALU-OutNone       | ->8 ->8 ->3 <-8
    ALU-OutResult     | ->8 ->8 ->3 <-8
    ALU-OutCarry      | ->8 ->8 ->3 <-8
    ALU-OutOr         | ->8 ->8 ->3 <-8
*/



class ComponentTest {
private:
    const int PROPAGATION_DELAY_MICROSECONDS = 100;

    Timer timer;
    unordered_map<string, GPIO::PinBlock> pin_blocks;
    unordered_map<string, vector<ComponentTestCase>> test_groups;

    auto Init() -> void;
    auto LoopTestResults(const vector<ComponentTestCase> &tests) -> void;
    auto PrintTestResults() -> void;

    auto RunTestCase(ComponentTestCase &test) -> void;
    auto RunTestGroup(vector<ComponentTestCase> &test_group) -> void;
    auto RunTestGroups() -> void;


public:
    ComponentTest(const unordered_map<string, GPIO::PinBlock> &pin_blocks, const unordered_map<string, vector<ComponentTestCase>> &test_groups) 
        : pin_blocks(pin_blocks), test_groups(test_groups) {}
    auto Run() -> void;
};