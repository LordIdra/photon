#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "Util.hpp"
#include "Util/Errors.hpp"



auto CheckErrorCode(const Errors::ErrorCode code) -> void {
    if (Errors::HasErrorCode() || Errors::GetErrorCode() != code) {
        cout << colors::CYAN + "Expected error code " << colors::BOLD_RED << code << 
                     colors::CYAN + " but got " << colors::BOLD_RED << Errors::GetErrorCode() << "\n";
        Errors::OutputErrors();
    }
    REQUIRE(Errors::GetErrorCode() == code);
}