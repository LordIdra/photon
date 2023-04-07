#include "gpio.cpp"

#include "../../util/logging.cpp"



void Verify(int addresses, int numbers, int iterations) {
    int errors = 0;
    
    // Loop once for every argument
    for (int address = 0; address < addresses; address++) {
        for (int number = 0; number < numbers; number++) {
            for (int iteration = 0; iteration < iterations; iteration++) {

                // Write data
                Write(address, number);

                // Read data
                unsigned char data = Read(address);

                // Check the data we just read matches the data we just wrote
                if (data != number) {
                    errors += 1;
                }
            }
        }
    }

    PrintErrors(errors);
}

int main (void) {
    Setup();
    
    // The purpose of this program is to make sure that every single bit in every single address
    // is written to and read from successfully without any errors
    Verify(8192, 128, 1);

    return 0;
}
