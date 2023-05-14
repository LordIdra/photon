#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include <Util/Util.hpp>
#include <Util/Colors.hpp>
#include <thread>
#include <wiringPi.h>



namespace Debug {
    namespace {
        GPIO::PinBlock block = GPIO::PinBlock{27, 1, OUTPUT};

        auto GetCommand() -> char {
            string command;
            while ((command != "") && (command != "s") && (command != "e")) {
                std::cout << YELLOW << "[ " << WHITE << "(press enter)" << NO_COLOR << " = next"
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

        auto SendClockPulse() -> void {
            GPIO::Set(block, TestNumber{1, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
            GPIO::Set(block, TestNumber{0, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
        }
    }

    auto DebugLoop() -> void {
        GPIO::SetupWiringPi();
        GPIO::SetupPinBlock(block);

        while (true) {
            const char command = GetCommand();

            if (command == 's') {
                const int count = GetSkipCount();
                for (int i = 0; i < count; i++) {
                    SendClockPulse();
                }
            }

            else if (command == 'e') {
                break;
            }

            else {
                SendClockPulse();
            }
        }
    }
}