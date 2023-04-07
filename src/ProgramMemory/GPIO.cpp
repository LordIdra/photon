#include <string>
#include <wiringPi.h>

#include <iostream>
#include <math.h>
#include <vector>

#include "../Util/Util.cpp"



int WE = 24;
int OE = 25;

int DATA_1 = 8;
int DATA_2 = 9;
int DATA_4 = 7;
int DATA_8 = 0;

int DATA_16 = 2;
int DATA_32 = 3;
int DATA_64 = 12;
int DATA_128 = 13;

int ADDR_1 = 15;
int ADDR_2 = 16;
int ADDR_4 = 1;
int ADDR_8 = 4;

int ADDR_16 = 5;
int ADDR_32 = 6;
int ADDR_64 = 10;
int ADDR_128 = 11;

int ADDR_256 = 31;
int ADDR_512 = 26;
int ADDR_1024 = 27;
int ADDR_2048 = 28;

int ADDR_4096 = 29;

// 128, 64, 32, etc
vector<int> data_pins = {8, 9, 7, 0, 2, 3, 12, 13};

// 4096, 2048, 1024, 512, etc
vector<int> address_pins = {15, 16, 1, 4, 5, 6, 10, 11, 31, 26, 27, 28, 29};



void Setup() {
    wiringPiSetup();
    
    pinMode(WE, OUTPUT);
    pinMode(OE, OUTPUT);

    pinMode(ADDR_4096, OUTPUT);

    pinMode(ADDR_2048, OUTPUT);
    pinMode(ADDR_1024, OUTPUT);
    pinMode(ADDR_512, OUTPUT);
    pinMode(ADDR_256, OUTPUT);

    pinMode(ADDR_128, OUTPUT);
    pinMode(ADDR_64, OUTPUT);
    pinMode(ADDR_32, OUTPUT);
    pinMode(ADDR_16, OUTPUT);

    pinMode(ADDR_8, OUTPUT);
    pinMode(ADDR_4, OUTPUT);
    pinMode(ADDR_2, OUTPUT);
    pinMode(ADDR_1, OUTPUT);
}

void SetAddress(unsigned int address_in) {
    vector<bool> address = DenaryToBinary(address_in, 13);

    digitalWrite(ADDR_4096, address[0]);
    
    digitalWrite(ADDR_2048, address[1]);
    digitalWrite(ADDR_1024, address[2]);
    digitalWrite(ADDR_512, address[3]);
    digitalWrite(ADDR_256, address[4]);

    digitalWrite(ADDR_128, address[5]);
    digitalWrite(ADDR_64, address[6]);
    digitalWrite(ADDR_32, address[7]);
    digitalWrite(ADDR_16, address[8]);

    digitalWrite(ADDR_8, address[9]);
    digitalWrite(ADDR_4, address[10]);
    digitalWrite(ADDR_2, address[11]);
    digitalWrite(ADDR_1, address[12]);
}

void SetData(unsigned char data_in) {
    bitfield data = DenaryToBinary(data_in, 8);

    pinMode(DATA_128, OUTPUT);
    pinMode(DATA_64, OUTPUT);
    pinMode(DATA_32, OUTPUT);
    pinMode(DATA_16, OUTPUT);

    pinMode(DATA_8, OUTPUT);
    pinMode(DATA_4, OUTPUT);
    pinMode(DATA_2, OUTPUT);
    pinMode(DATA_1, OUTPUT);

    digitalWrite(DATA_128, data[0]);
    digitalWrite(DATA_64, data[1]);
    digitalWrite(DATA_32, data[2]);
    digitalWrite(DATA_16, data[3]);

    digitalWrite(DATA_8, data[4]);
    digitalWrite(DATA_4, data[5]);
    digitalWrite(DATA_2, data[6]);
    digitalWrite(DATA_1, data[7]);
}

bitfield ReadData() {
    bitfield data;
    data.reserve(8); // tells the computer we're about to write 8 values, speeds the process up slightly

    pinMode(DATA_128, INPUT);
    pinMode(DATA_64, INPUT);
    pinMode(DATA_32, INPUT);
    pinMode(DATA_16, INPUT);

    pinMode(DATA_8, INPUT);
    pinMode(DATA_4, INPUT);
    pinMode(DATA_2, INPUT);
    pinMode(DATA_1, INPUT);
    
    data.push_back(digitalRead(DATA_128));
    data.push_back(digitalRead(DATA_64));
    data.push_back(digitalRead(DATA_32));
    data.push_back(digitalRead(DATA_16));

    data.push_back(digitalRead(DATA_8));
    data.push_back(digitalRead(DATA_4));
    data.push_back(digitalRead(DATA_2));
    data.push_back(digitalRead(DATA_1));

    return data;
}

void Write(unsigned int address, unsigned char data) {
    SetAddress(address);
    SetData(data);
    
    digitalWrite(OE, HIGH);
    digitalWrite(WE, LOW);
}

unsigned char Read(unsigned int address) {
    SetAddress(address);
    
    digitalWrite(OE, LOW);
    digitalWrite(WE, HIGH);

    return BinaryToDenary(ReadData());
}
