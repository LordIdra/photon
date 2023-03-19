#include "Util/Errors.hpp"
#include <catch2/catch_test_macros.hpp>

#include <Files/Files.hpp>



TEST_CASE("[RFL] Invalid File") {
    REQUIRE_THROWS_AS(Files::Read("this file does not exist"), Errors::Files::NotFound);
}

TEST_CASE("[RFL] List Files") {
    vector<string> expected_names = {
        "hello.txt",
        "hi.txt"
    };

    vector<string> actual_names = Files::FilePaths("../../resources/files");
    
    REQUIRE(expected_names == actual_names);
}

TEST_CASE("[RFL] Read") {
    vector<string> expectedLines = {
        "Hi mate, my name's John. I come from the land of the free.",
        "",
        "In ma country, we don't have no SLT to impose no nonsense on us. We're free men yo, and we do whatever we want.",
        "",
        "",
        "",
        "",
        "",
        "Ay"
    };

    vector<string> actualLines = Files::Read("../../resources/files/hello.txt");
    
    REQUIRE(expectedLines == actualLines);
}

TEST_CASE("[RFL] Write") {
    vector<string> lines1 = {
        "Nothing to",
        "see here"
    };

    vector<string> lines2 = {
        "Mr Bayeh is the G.O.A.T"
    };

    Files::Write("../../resources/files/hi.txt", lines1);
    vector<string> actual_lines1 = Files::Read("../../resources/files/hi.txt");
    REQUIRE(lines1 == actual_lines1);

    Files::Write("../../resources/files/hi.txt", lines2);
    vector<string> actual_lines2 = Files::Read("../../resources/files/hi.txt");
    REQUIRE(lines2 == actual_lines2);
}