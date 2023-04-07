#include "gpio.cpp"

#include "../../util/logging.cpp"
#include "../../util/read_hex.cpp"



const int UPDATE_COUNT = 10;

void CheckArgumentCount(int argc) {
    // Check that there are 3 arguments
    if (argc != 3) {
        Error("Usage: write [filename] [start location]");
        exit(1);
    }
}

void CheckSecondArgumentIsInt(char *argv[]) {
    // Check that the second argument is a number
    if (!IsNumber(argv[2])) {
        Error("Invalid Number: " + (string)argv[2]);
    }
}

void CheckDataNotEmpty(vector<unsigned char> data, string filename) {
    // Check that any data can actually be read
    // If not, the file most likely just does not exist (or, to give an edge case, is empty)
    if (data == vector<unsigned char>()) {
        Error("File Not Found: " + filename);
        exit(1);
    }
}

vector<unsigned char> GetProgram(int argc, char *argv[]) {
    CheckArgumentCount(argc);
    CheckSecondArgumentIsInt(argv);

    string filename = argv[1];
    vector<unsigned char> data = ReadHex(filename);
    
    CheckDataNotEmpty(data, filename);

    return data;
}

void WriteProgram(vector<unsigned char> program, unsigned int offset, unsigned int length) {
    // Loop through every byte
    for (unsigned int address = offset; address < length + offset; address++) {
        unsigned char data = program[address];

        // Write the byte
        Write(address, data);
    }
}

void VerifyProgram(vector<unsigned char> program, unsigned int offset, unsigned int length) {
    // Loop through every byte
    unsigned int errors = 0;
    for (unsigned int address = offset; address < length + offset; address++) {
        unsigned char data = program[address];

        // Read the byte
        unsigned char result = Read(address);

        // Compare the expected and actual values, increment error counter if a discrepancy is found
        if (result != data) {
            errors++;
        }
    }

    PrintErrors(errors);
}

int main(int argc, char *argv[]) {
    vector<unsigned char> program = GetProgram(argc, argv);  // program itself
    unsigned int offset = atoi(argv[2]);  // program offset (where the start of the program is in the EEPROM)
    unsigned int length = program.size();  // program length (in bytes)

    Setup();
    WriteProgram(program, offset, length);
    VerifyProgram(program, offset, length);
}
