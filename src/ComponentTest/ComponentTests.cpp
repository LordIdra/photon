#include "ComponentTest.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include <wiringPi.h>




namespace ComponentTests {
    namespace Generate {
        auto MemAdder() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Data_in", GPIO::PinBlock{0, 12, OUTPUT}));
            pin_blocks.insert(make_pair("Data_out", GPIO::PinBlock{12, 12, INPUT}));

            vector<ComponentTestCase> group_main;
            group_main.reserve(4095);
            for (int x = 0; x < 4095; x++) {
                 group_main.push_back(ComponentTestCase{ 
                    {{"Data_in", x}}, 
                    {{"Data_out", x+1}} });
            }
            test_groups.insert(make_pair("main", group_main));

            return ComponentTest(pin_blocks, test_groups);
        }

        auto RegisterFile() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Clk", GPIO::PinBlock{0, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Select", GPIO::PinBlock{1, 4, OUTPUT}));
            pin_blocks.insert(make_pair("Data_in", GPIO::PinBlock{5, 8, OUTPUT}));
            pin_blocks.insert(make_pair("Test_out", GPIO::PinBlock{13, 8, INPUT}));

            vector<ComponentTestCase> group_one_register_all_values;
            for (int reg = 1; reg < 15; reg++) {
                for (int val = -128; val < 128; val++) {
                    group_one_register_all_values.push_back(ComponentTestCase{ 
                       {{"Clk", 0}, {"Select", reg}, {"Data_in", val}}, 
                       {} });
                    group_one_register_all_values.push_back(ComponentTestCase{ 
                        {{"Clk", 1}, {"Select", reg}, {"Data_in", val}}, 
                        {{"Test_out", val}} });
                }
            }
            test_groups.insert(make_pair("All registers one value", group_one_register_all_values));

            vector<ComponentTestCase> group_all_registers_one_value;
            for (int val = -128; val < 128; val++) {
                for (int reg = 1; reg < 15; reg++) {
                    group_all_registers_one_value.push_back(ComponentTestCase{ 
                       {{"Clk", 0}, {"Select", reg}, {"Data_in", val}}, 
                       {} });
                    group_all_registers_one_value.push_back(ComponentTestCase{ 
                        {{"Clk", 1}, {"Select", reg}, {"Data_in", val}}, 
                        {{"Test_out", val}} });
                }
            }
            test_groups.insert(make_pair("One register all values", group_all_registers_one_value));

            return ComponentTest(pin_blocks, test_groups);
        }

        auto SCU() -> ComponentTest {
            // TODO
            
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            return ComponentTest(pin_blocks, test_groups);
        }

        auto ALU_OutFlags() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Data-1_in", GPIO::PinBlock{0, 8, OUTPUT}));
            pin_blocks.insert(make_pair("Data-2_in", GPIO::PinBlock{8, 8, OUTPUT}));
            
            pin_blocks.insert(make_pair("Invert", GPIO::PinBlock{16, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Increment", GPIO::PinBlock{17, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Propagate", GPIO::PinBlock{18, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Write_flags", GPIO::PinBlock{19, 1, OUTPUT}));

            pin_blocks.insert(make_pair("Greater", GPIO::PinBlock{20, 1, INPUT}));
            pin_blocks.insert(make_pair("Less", GPIO::PinBlock{21, 1, INPUT}));
            pin_blocks.insert(make_pair("Equal", GPIO::PinBlock{22, 1, INPUT}));
            pin_blocks.insert(make_pair("NotEqual", GPIO::PinBlock{23, 1, INPUT}));

            vector<ComponentTestCase> group_test;
            for (int val = -128; val < 128; val++) {
                group_test.push_back(ComponentTestCase{ 
                    {{"Data-1_in", val}, {"Data-2_in", 0}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 0}, {"Write_flags", 0}}, 
                    {} });
                group_test.push_back(ComponentTestCase{ 
                    {{"Data-1_in", val}, {"Data-2_in", 0}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 0}, {"Write_flags", 1}}, 
                    {{"Greater", val > 0}, {"Less", val < 0}, {"Equal", val == 0}, {"NotEqual", val != 0}} });
            }
            test_groups.insert(make_pair("Test", group_test));

            vector<ComponentTestCase> group_compare;
            for (int a = -128; a < 128; a++) {
                for (int b = -128; b < 128; b++) {
                    group_compare.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 1}, {"Increment", 1}, {"Propagate", 1}, {"Write_flags", 0}}, 
                        {} });
                    group_compare.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 0}, {"Write_flags", 1}}, 
                        {{"Greater", a > b}, {"Less", a < b}, {"Equal", a == b}, {"NotEqual", a != b}} });
                }
            }
            test_groups.insert(make_pair("Compare", group_compare));

            return ComponentTest(pin_blocks, test_groups);
        }

        auto ALU_OutResult() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Data-1_in", GPIO::PinBlock{0, 8, OUTPUT}));
            pin_blocks.insert(make_pair("Data-2_in", GPIO::PinBlock{8, 8, OUTPUT}));

            pin_blocks.insert(make_pair("Invert", GPIO::PinBlock{16, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Increment", GPIO::PinBlock{17, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Propagate", GPIO::PinBlock{18, 1, OUTPUT}));

            pin_blocks.insert(make_pair("Carry", GPIO::PinBlock{19, 1, INPUT}));
            pin_blocks.insert(make_pair("Data_out", GPIO::PinBlock{20, 8, INPUT}));

            vector<ComponentTestCase> group_add;
            for (int a = -128; a < 128; a++) {
                for (int b = -128; b < 128; b++) {
                    const bool carry = ((a+b)>127) || (a+b<-128);
                    
                    int result = a+b;
                    if (result > 127) {
                        result -= 256;
                    } else if (result < -128) {
                        result += 256;
                    }

                    group_add.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 1}}, 
                        {{"Carry", carry}, {"Data_out", result}} });
                }
            }
            test_groups.insert(make_pair("Add", group_add));

            vector<ComponentTestCase> group_sub;
            for (int a = -128; a < 128; a++) {
                for (int b = -128; b < 128; b++) {
                    const bool carry = !( ((a+b)>127) || (a+b<-128) );

                    int result = a-b;
                    if (result > 127) {
                        result -= 256;
                    } else if (result < -128) {
                        result += 256;
                    }

                    group_sub.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 1}, {"Increment", 1}, {"Propagate", 1}},
                        {{"Carry", carry}, {"Data_out", result}} });
                }
            }
            test_groups.insert(make_pair("Subtract", group_sub));

            vector<ComponentTestCase> group_inc;
            for (int a = -128; a < 127; a++) {
                group_inc.push_back(ComponentTestCase{ 
                    {{"Data-1_in", a}, {"Data-2_in", 0}, {"Invert", 0}, {"Increment", 1}, {"Propagate", 1}},
                    {{"Carry", 0}, {"Data_out", a+1}} });
            }
            test_groups.insert(make_pair("Increment", group_inc));

            vector<ComponentTestCase> group_dec;
            for (int a = -127; a < 128; a++) {
                group_dec.push_back(ComponentTestCase{ 
                    {{"Data-1_in", a}, {"Data-2_in", 0}, {"Invert", 1}, {"Increment", 0}, {"Propagate", 1}},
                    {{"Carry", 0}, {"Data_out", a-1}} });
            }
            test_groups.insert(make_pair("Decrement", group_dec));

            vector<ComponentTestCase> group_inv;
            for (int b = -127; b < 128; b++) {
                group_inv.push_back(ComponentTestCase{ 
                    {{"Data-1_in", 0}, {"Data-2_in", b}, {"Invert", 1}, {"Increment", 1}, {"Propagate", 1}},
                    {{"Data_out", -b}} });
            }
            test_groups.insert(make_pair("Invert", group_inv));

            vector<ComponentTestCase> group_not;
            for (int b = -127; b < 128; b++) {
                group_not.push_back(ComponentTestCase{ 
                    {{"Data-1_in", 0}, {"Data-2_in", b}, {"Invert", 1}, {"Increment", 0}, {"Propagate", 0}},
                    {{"Data_out", -b-1}} });
            }
            test_groups.insert(make_pair("Not", group_not));

            vector<ComponentTestCase> group_xor;
            for (int a = -127; a < 128; a++) {
                for (int b = -127; b < 128; b++) {
                    group_xor.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 0}},
                        {{"Data_out", a^b}} });
                }
            }
            test_groups.insert(make_pair("Xor", group_xor));

            return ComponentTest(pin_blocks, test_groups);
        }

        auto ALU_OutCarry() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Data-1_in", GPIO::PinBlock{0, 8, OUTPUT}));
            pin_blocks.insert(make_pair("Data-2_in", GPIO::PinBlock{8, 8, OUTPUT}));

            pin_blocks.insert(make_pair("Invert", GPIO::PinBlock{16, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Increment", GPIO::PinBlock{17, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Propagate", GPIO::PinBlock{18, 1, OUTPUT}));

            pin_blocks.insert(make_pair("Carry", GPIO::PinBlock{19, 1, INPUT}));
            pin_blocks.insert(make_pair("Data_out", GPIO::PinBlock{20, 8, INPUT}));

            vector<ComponentTestCase> group_and;
            for (int a = -128; a < 128; a++) {
                for (int b = -128; b < 128; b++) {
                    group_and.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 0}}, 
                        {{"Data_out", a&b}} });
                }
            }
            test_groups.insert(make_pair("And", group_and));

            return ComponentTest(pin_blocks, test_groups);
        }

        auto ALU_OutOr() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Data-1_in", GPIO::PinBlock{0, 8, OUTPUT}));
            pin_blocks.insert(make_pair("Data-2_in", GPIO::PinBlock{8, 8, OUTPUT}));

            pin_blocks.insert(make_pair("Invert", GPIO::PinBlock{16, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Increment", GPIO::PinBlock{17, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Propagate", GPIO::PinBlock{18, 1, OUTPUT}));

            pin_blocks.insert(make_pair("Carry", GPIO::PinBlock{19, 1, INPUT}));
            pin_blocks.insert(make_pair("Data_out", GPIO::PinBlock{20, 8, INPUT}));

            vector<ComponentTestCase> group_or;
            for (int a = -128; a < 128; a++) {
                for (int b = -128; b < 128; b++) {
                    group_or.push_back(ComponentTestCase{ 
                        {{"Data-1_in", a}, {"Data-2_in", b}, {"Invert", 0}, {"Increment", 0}, {"Propagate", 0}}, 
                        {{"Data_out", a|b}} });
                }
            }
            test_groups.insert(make_pair("Or", group_or));

            return ComponentTest(pin_blocks, test_groups);
        }
    }

    auto TestMemAdder() -> void {
        Generate::MemAdder().Run();
    }

    auto TestSCU() -> void {
        Generate::SCU().Run();
    }

    auto TestRegisterFile() -> void {
        Generate::RegisterFile().Run();
    }

    auto TestALU_OutFlags() -> void {
        Generate::ALU_OutFlags().Run();
    }

    auto TestALU_OutResult() -> void {
        Generate::ALU_OutResult().Run();
    }

    auto TestALU_OutCarry() -> void {
        Generate::ALU_OutCarry().Run();
    }

    auto TestALU_OutOr() -> void {
        Generate::ALU_OutOr().Run();
    }

}