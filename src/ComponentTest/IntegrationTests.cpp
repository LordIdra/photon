#include "Assembler/Assembler.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include <ComponentTest/IntegrationTests.hpp>
#include <Util/Colors.hpp>
#include <thread>
#include <wiringPi.h>



namespace IntegrationTests {
    namespace {
        GPIO::PinBlock data_block = GPIO::PinBlock{12, 8, OUTPUT};
        GPIO::PinBlock clock_block = GPIO::PinBlock{27, 1, OUTPUT};

        int line = 1;

        auto GetCommand() -> char {
            string command;
            while (command != "n") {
                std::cout << YELLOW << "[ " << WHITE << "n" << NO_COLOR << " = next"
                        << YELLOW << " ]" << NO_COLOR << "\n";
                std::cin >> command;
            }
            return command.at(0);
        }

        auto ClockPulse(const bool debug_mode) -> void {
            if (debug_mode) {
                GetCommand();
            }
            GPIO::Set(clock_block, TestNumber{1, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
            GPIO::Set(clock_block, TestNumber{0, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
        }

        auto Run(const bool debug_mode, const pair<int, int> &instruction) -> void {
            GPIO::Set(data_block, TestNumber{instruction.first, false});
            ClockPulse(debug_mode);
            GPIO::Set(data_block, TestNumber{instruction.second, false});
            ClockPulse(debug_mode);
            ClockPulse(debug_mode);
            ClockPulse(debug_mode);
            ClockPulse(debug_mode);
            ClockPulse(debug_mode);
            ClockPulse(debug_mode);
            ClockPulse(debug_mode);
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

    auto RunInstructions(const bool debug_mode, const vector<pair<int, int>> &instructions) -> void {
        Init();
        for (const pair<int, int> &instruction : instructions) {
            Run(debug_mode, instruction);
        }
    }
}