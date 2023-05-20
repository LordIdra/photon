#include "ComponentTest.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include <wiringPi.h>




namespace ComponentTests {
    auto TestMemAdderAndRPC() -> void;
    auto TestRegisterFile() -> void;
    auto TestALU_OutFlags() -> void;
    auto TestALU_OutResult() -> void;
    auto TestALU_OutCarry() -> void;
    auto TestALU_OutOr() -> void;
}