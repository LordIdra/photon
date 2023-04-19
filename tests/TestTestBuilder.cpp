#include "Util/Errors.hpp"
#include <cassert>
#include <catch2/catch_test_macros.hpp>

#include <SimulationTestBuilder/SimulationTestBuilder.hpp>
#include <Util/Util.hpp>
#include <stdexcept>



TEST_CASE("[TBD] Nothing to expect") {
    vector<string> input = {
        "!2",
        "SET 2 r1",
    };

    REQUIRE_THROWS_AS(SimulationTestBuilder::Build("TEST", input), Errors::TestBuilder::NothingToExpect);
}

TEST_CASE("[TBD] Valid 1") {
    vector<string> input = {
        "SET 2 r1",
        "!2",
        "SET 87 r1",
        "!87",
        "~SET -7 r1",
    };

    vector<string> expected_output = {
        "Tick Pulse Test_out",
        "# SET 2 r1",
        "memory ProgramMemory(0)=240;",
        "memory ProgramMemory(1)=33;",
        "# SET 87 r1",
        "memory ProgramMemory(2)=245;",
        "memory ProgramMemory(3)=113;",
        "# SET -7 r1",
        "memory ProgramMemory(4)=255;",
        "memory ProgramMemory(5)=145;",
        "# SET 2 r1",
        "1 1 X",
        "1 0 X",
        "2 1 X",
        "2 0 X",
        "3 1 X",
        "3 0 X",
        "4 1 X",
        "4 0 X",
        "5 1 X",
        "5 0 X",
        "6 1 X",
        "6 0 X",
        "7 1 X",
        "7 0 X",
        "8 1 X",
        "8 0 (2)",
        "# SET 87 r1",
        "1 1 X",
        "1 0 X",
        "2 1 X",
        "2 0 X",
        "3 1 X",
        "3 0 X",
        "4 1 X",
        "4 0 X",
        "5 1 X",
        "5 0 X",
        "6 1 X",
        "6 0 X",
        "7 1 X",
        "7 0 X",
        "8 1 X",
        "8 0 (87)",
    };

    vector<string> actual_output = SimulationTestBuilder::Build("TEST", input);

    REQUIRE(actual_output.size() == expected_output.size());

    for (int i = 0; i < actual_output.size(); i++) {
        REQUIRE(actual_output.at(i) == expected_output.at(i));
    }
}

TEST_CASE("[TBD] Valid 2") {
    vector<string> input = {
        "SET 2 r4",
        "!0",
        "MOV r1 r4",
        "!2",
    };

    vector<string> expected_output = {
        "Tick Pulse Test_out",
        "# SET 2 r4",
        "memory ProgramMemory(0)=240;",
        "memory ProgramMemory(1)=36;",
        "# MOV r1 r4",
        "memory ProgramMemory(2)=33;",
        "memory ProgramMemory(3)=65;",
        "# SET 2 r4",
        "1 1 X",
        "1 0 X",
        "2 1 X",
        "2 0 X",
        "3 1 X",
        "3 0 X",
        "4 1 X",
        "4 0 X",
        "5 1 X",
        "5 0 X",
        "6 1 X",
        "6 0 X",
        "7 1 X",
        "7 0 X",
        "8 1 X",
        "8 0 (0)",
        "# MOV r1 r4",
        "1 1 X",
        "1 0 X",
        "2 1 X",
        "2 0 X",
        "3 1 X",
        "3 0 X",
        "4 1 X",
        "4 0 X",
        "5 1 X",
        "5 0 X",
        "6 1 X",
        "6 0 X",
        "7 1 X",
        "7 0 X",
        "8 1 X",
        "8 0 (2)",
    };

    vector<string> actual_output = SimulationTestBuilder::Build("TEST", input);

    REQUIRE(actual_output.size() == expected_output.size());

    for (int i = 0; i < actual_output.size(); i++) {
        REQUIRE(actual_output.at(i) == expected_output.at(i));
    }
}
