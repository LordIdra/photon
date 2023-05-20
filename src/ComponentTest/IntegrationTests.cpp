#include "Assembler/Assembler.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include <ComponentTest/IntegrationTests.hpp>
#include <thread>
#include <wiringPi.h>



namespace IntegrationTests {
    namespace {
        GPIO::PinBlock data_block = GPIO::PinBlock{12, 8, OUTPUT};
        GPIO::PinBlock clock_block = GPIO::PinBlock{27, 1, OUTPUT};

        int line = 1;

        auto ClockPulse() -> void {
            GPIO::Set(clock_block, TestNumber{1, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
            GPIO::Set(clock_block, TestNumber{0, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
        }

        auto Run(const pair<int, int> &instruction) -> void {
            GPIO::Set(data_block, TestNumber{instruction.first, false});
            ClockPulse();
            GPIO::Set(data_block, TestNumber{instruction.second, false});
            ClockPulse();
            ClockPulse();
            ClockPulse();
            ClockPulse();
            ClockPulse();
            ClockPulse();
            ClockPulse();
            line++;
        }

        auto Init() -> void {
            GPIO::SetupWiringPi();
            GPIO::SetupPinBlock(data_block);
            GPIO::SetupPinBlock(clock_block);
        }
    }
    
    auto ResetTestPins() -> void {
        Init();
        GPIO::Set(data_block, TestNumber{0, false});
        GPIO::Set(data_block, TestNumber{0, false});
    }

    auto RunInstructions(const vector<pair<int, int>> &instructions) -> void {
        Init();
        for (const pair<int, int> &instruction : instructions) {
            Run(instruction);
        }
    }
}