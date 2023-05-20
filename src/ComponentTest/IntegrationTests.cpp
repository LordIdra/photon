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
            while ((command != "n") && (command != "s") && (command != "e")) {
                std::cout << YELLOW << "[ " << WHITE << "n" << NO_COLOR << " = next"
                        << YELLOW << " | " << WHITE << "s" << NO_COLOR << " = skip"
                        << YELLOW << " | " << WHITE << "e" << NO_COLOR << " = exit"
                        << YELLOW << " ]" << NO_COLOR << "\n";
                std::cin >> command;
            }
            return command.at(0);
        }

        auto GetSkipCount() -> int {
            cout << WHITE << "Number of pulses to skip" << CYAN << "\n";
            string test_string;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> test_string;
            return std::stoi(test_string);
        }

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

    auto RunInstructions(const bool debug_mode, const vector<pair<int, int>> &instructions) -> void {
        Init();
        int to_skip = 0;
        for (const pair<int, int> &instruction : instructions) {
            if (debug_mode && (to_skip == 0)) {
                const char command = GetCommand();
                if (command == 'n') {
                    continue;
                }

                else if (command == 's') {
                    const int count = GetSkipCount();
                    to_skip = count;
                }
            }
            to_skip--;
            Run(instruction);
        }
    }
}