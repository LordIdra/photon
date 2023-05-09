#include "../Electronics/GPIO.hpp"

#include "ComponentTest/ComponentTestCase.hpp"
#include "Util/Util.hpp"
#include "Util/Colors.hpp"
#include <thread>
#include <wiringPi.h>



namespace ProgramMemory {
    namespace {
        const GPIO::PinBlock block_address {0, 12, OUTPUT};
        const GPIO::PinBlock block_data_out {12, 8, OUTPUT};
        const GPIO::PinBlock block_data_in {12, 8, INPUT};
        const GPIO::PinBlock block_not_write_enable {20, 1, OUTPUT};
        const GPIO::PinBlock block_not_output_enable {21, 1, OUTPUT};

        auto SetModeRead() -> void {
            GPIO::Set(block_not_output_enable, TestNumber{0});
            GPIO::Set(block_not_write_enable, TestNumber{1});
        }

        auto SetModeWrite() -> void {
            GPIO::Set(block_not_output_enable, TestNumber{1});
            GPIO::Set(block_not_write_enable, TestNumber{0});
        }

        auto Write(const int address, const int value) -> void {
            GPIO::Set(block_address, TestNumber{address, false});
            GPIO::SetupPinBlock(block_data_out);
            std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));
            GPIO::Set(block_data_out, TestNumber{value, true});
            std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));
            SetModeWrite();
        }

        auto Read(const int address) -> unsigned int {
            SetModeRead();
            GPIO::SetupPinBlock(block_data_in);
            std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));
            GPIO::Set(block_address, TestNumber{address, false});
            std::this_thread::sleep_for(std::chrono::microseconds(PROPAGATION_DELAY_MICROSECONDS));
            return GPIO::ReadInt(block_data_in, true);
        }

        void PrintErrors(const int errors) {
            cout << WHITE << "Completed with ";
            if (errors == 0) {
                cout << GREEN << 0;
            } else {
                cout << RED << errors;
            }
            cout << WHITE << " errors" << "\n";
        }
    }

    auto Test() -> void {
        GPIO::SetupWiringPi();
        GPIO::SetupPinBlock(block_address);
        GPIO::SetupPinBlock(block_not_write_enable);
        GPIO::SetupPinBlock(block_not_output_enable);

        GPIO::Set(block_not_write_enable, TestNumber{0});
        GPIO::Set(block_not_output_enable, TestNumber{0});

        int errors = 0;
        
        for (int address = 0; address < 8192; address++) {
            for (int number = 0; number < 128; number++) {
                Write(address, number);
                const unsigned int data = Read(address);
                if (data != number) {
                    errors ++;
                }
            }
        }

        PrintErrors(errors);
    }
}