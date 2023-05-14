#include "../Electronics/GPIO.hpp"

#include "ComponentTest/ComponentTestCase.hpp"
#include "Util/Util.hpp"
#include "Util/Colors.hpp"
#include "Util/Timer.hpp"
#include <thread>
#include <wiringPi.h>



namespace ProgramMemory {
    namespace {
        const GPIO::PinBlock block_address {0, 12, OUTPUT};
        const GPIO::PinBlock block_data_out {12, 8, OUTPUT};
        const GPIO::PinBlock block_data_in {12, 8, INPUT};
        const GPIO::PinBlock block_not_write_enable {20, 1, OUTPUT};
        const GPIO::PinBlock block_not_output_enable {21, 1, OUTPUT};

        Timer timer;

        auto SetModeRead() -> void {
            GPIO::Set(block_not_output_enable, TestNumber{0});
            GPIO::Set(block_not_write_enable, TestNumber{1});
        }

        auto SetModeWrite() -> void {
            GPIO::Set(block_not_output_enable, TestNumber{1});
            GPIO::Set(block_not_write_enable, TestNumber{0});
        }

        auto WriteData(const int address, const int value) -> void {
            GPIO::Set(block_address, TestNumber{address, false});
            GPIO::SetupPinBlock(block_data_out);
            GPIO::Set(block_data_out, TestNumber{value, false});
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
            SetModeWrite();
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
            SetModeRead();
            std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_MICROSECONDS));
        }

        auto ReadData(const int address) -> int {
            SetModeRead();
            GPIO::SetupPinBlock(block_data_in);
            std::this_thread::sleep_for(std::chrono::microseconds(READ_DELAY_MICROSECONDS));
            GPIO::Set(block_address, TestNumber{address, false});
            std::this_thread::sleep_for(std::chrono::microseconds(READ_DELAY_MICROSECONDS));
            return GPIO::ReadInt(block_data_in, false);
        }

        void PrintWriteComplete() {
            cout << WHITE << "Write completed in " << CYAN << timer.GetMilliseconds() << "ms" << WHITE << "\n";
        }

        void PrintErrors(const int errors) {
            cout << WHITE << "Completed in " << CYAN << timer.GetMilliseconds() << "ms" << WHITE << " with ";
            if (errors == 0) {
                cout << GREEN << 0;
            } else {
                cout << RED << errors;
            }
            cout << WHITE << " errors" << "\n";
        }
    }

    auto Test() -> void {
        timer.Start();

        GPIO::SetupWiringPi();
        GPIO::SetupPinBlock(block_address);
        GPIO::SetupPinBlock(block_not_write_enable);
        GPIO::SetupPinBlock(block_not_output_enable);

        int errors = 0;
        
        for (int address = 0; address < 4096; address++) {
            for (int number = 0; number < 3; number++) {
                WriteData(address, number);
                const int data = ReadData(address);
                if (data != number) {
                    std::cout << WHITE << "[Address " << CYAN << address << WHITE << "] " 
                              << RED << "Expected " << YELLOW << number 
                              << RED << " but got " << YELLOW << data << "\n";
                    errors ++;
                }
            }
        }

        timer.Stop();

        PrintErrors(errors);
    }

    auto Write(const vector<int> &assembly) -> void {
        GPIO::SetupWiringPi();
        GPIO::SetupPinBlock(block_address);
        GPIO::SetupPinBlock(block_not_write_enable);
        GPIO::SetupPinBlock(block_not_output_enable);

        // Write pass
        timer.Start();
        for (int address = 0; address < assembly.size(); address++) {
            WriteData(address, assembly.at(address));
        }
        timer.Stop();
        PrintWriteComplete();

        // Verify pass
        timer.Start();
        int errors = 0;
        for (int address = 0; address < assembly.size(); address++) {
            const int expected = assembly.at(address);
            const int actual = ReadData(address);
            if (actual != expected) {
                std::cout << WHITE << "[Address " << CYAN << address << WHITE << "] " 
                          << RED << "Expected " << YELLOW << expected 
                          << RED << " but got " << YELLOW << actual << "\n";
                errors ++;
            }
        }
        timer.Stop();
        PrintErrors(errors);
    }
}