#include "Util/Errors.hpp"
#include <catch2/catch_test_macros.hpp>

#include <Files/Files.hpp>



TEST_CASE("[1|RFL] Invalid File") {
    try {
        vector<string> actualLines = Files::Read("this file does not exist");
    } catch (Errors::Files::NotFound &e) {
        REQUIRE(true);
        return;
    } catch (const std::runtime_error &e) {}
    REQUIRE(false);
}

TEST_CASE("[1|RFL] Success 1") {
    vector<string> actualLines = Files::Read("../../resources/hello.txt");
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
    REQUIRE(expectedLines == actualLines);
}