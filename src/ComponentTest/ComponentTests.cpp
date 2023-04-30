#include "ComponentTest.hpp"
#include "ComponentTest/ComponentTestCase.hpp"
#include "Electronics/GPIO.hpp"
#include <wiringPi.h>




namespace ComponentTests {
    namespace Generate {
        auto MemAdderAndRPC() -> ComponentTest {
            unordered_map<string, GPIO::PinBlock> pin_blocks;
            unordered_map<string, vector<ComponentTestCase>> test_groups;

            pin_blocks.insert(make_pair("Clk_write", GPIO::PinBlock{0, 1, OUTPUT}));
            pin_blocks.insert(make_pair("Data_out", GPIO::PinBlock{1, 12, INPUT}));

            vector<ComponentTestCase> group_main;
            group_main.reserve(4095);

            for (int x = 2; x < 4096; x++) {
                group_main.push_back(ComponentTestCase{ 
                    {{"Clk_write", TestNumber{0, false}}}, 
                });

                 group_main.push_back(ComponentTestCase{ 
                    {{"Clk_write", TestNumber{1, false}}}, 
                    {{"Data_out", TestNumber{x, false}}}
                });
            }

            // Wrap the output back round to 1 in case the test has to be run multiple times (two clock pulses since we started at x=2)
            group_main.push_back(ComponentTestCase{{{"Clk_write", TestNumber{0, false}}}});
            group_main.push_back(ComponentTestCase{{{"Clk_write", TestNumber{1, false}}}});
            group_main.push_back(ComponentTestCase{{{"Clk_write", TestNumber{0, false}}}});
            group_main.push_back(ComponentTestCase{{{"Clk_write", TestNumber{1, false}}}});

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
                        {
                            {"Clk", TestNumber{0, false}}, 
                            {"Select", TestNumber{reg, false}}, 
                            {"Data_in", TestNumber{val, true}}
                        }, 
                        {} 
                    });

                    group_one_register_all_values.push_back(ComponentTestCase{ 
                        {
                            {"Clk", TestNumber{1, false}}, 
                            {"Select", TestNumber{reg, false}}, 
                            {"Data_in", TestNumber{val, true}}
                        }, 
                        {
                            {"Test_out", TestNumber{val, true}}
                        } 
                    });
                }
            }

            test_groups.insert(make_pair("All registers one value", group_one_register_all_values));

            vector<ComponentTestCase> group_all_registers_one_value;

            for (int val = -128; val < 128; val++) {
                for (int reg = 1; reg < 15; reg++) {
                    group_all_registers_one_value.push_back(ComponentTestCase{ 
                        {
                            {"Clk", TestNumber{0, false}}, 
                            {"Select", TestNumber{reg, false}}, 
                            {"Data_in", TestNumber{val, true}}}, 
                        {} 
                    });

                    group_all_registers_one_value.push_back(ComponentTestCase{ 
                        {
                            {"Clk", TestNumber{1, false}}, 
                            {"Select", TestNumber{reg, false}}, 
                            {"Data_in", TestNumber{val, true}}
                        }, 
                        {
                            {"Test_out", TestNumber{val, true}}
                        } 
                    });
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
                    {
                        {"Data-1_in", TestNumber{val, true}}, 
                        {"Data-2_in", TestNumber{0, false}}, 
                        {"Invert", TestNumber{0, false}}, 
                        {"Increment", TestNumber{0, false}}, 
                        {"Propagate", TestNumber{0, false}}, 
                        {"Write_flags", TestNumber{0, false}}}, 
                    {} 
                });

                group_test.push_back(ComponentTestCase{ 
                    {
                        {"Data-1_in", TestNumber{val, true}}, 
                        {"Data-2_in", TestNumber{0, false}}, 
                        {"Invert", TestNumber{0, false}}, 
                        {"Increment", TestNumber{0, false}}, 
                        {"Propagate", TestNumber{0, false}}, 
                        {"Write_flags", TestNumber{1, false}}
                    }, 
                    {
                        {"Greater", TestNumber{val > 0, false}}, 
                        {"Less", TestNumber{val < 0, false}}, 
                        {"Equal", TestNumber{val == 0, false}}, 
                        {"NotEqual", TestNumber{val != 0, false}}
                    }
                });
            }
            test_groups.insert(make_pair("Test", group_test));

            vector<ComponentTestCase> group_compare;
            for (int a = -128; a < 128; a++) {
                for (int b = -128; b < 128; b++) {
                    group_compare.push_back(ComponentTestCase{ 
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{1, false}}, 
                            {"Increment", TestNumber{1, false}}, 
                            {"Propagate", TestNumber{1, false}}, 
                            {"Write_flags", TestNumber{0, false}}}, 
                        {} 
                    });

                    group_compare.push_back(ComponentTestCase{ 
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{0, false}}, 
                            {"Increment", TestNumber{0, false}}, 
                            {"Propagate", TestNumber{0, false}}, 
                            {"Write_flags", TestNumber{1, false}}
                        }, 
                        {
                            {"Greater", TestNumber{a > b, false}}, 
                            {"Less", TestNumber{a < b, false}}, 
                            {"Equal", TestNumber{a == b, false}}, 
                            {"NotEqual", TestNumber{a != b, false}}
                        } 
                    });
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
            for (int a = -128; a < 128; a+=33) {
                for (int b = -128; b < 128; b+=33) {
                    int unsigned_a = a;
                    int unsigned_b = b;

                    if (unsigned_a < 0) {
                        unsigned_a += 256;
                    }
                    if (unsigned_b < 0) {
                        unsigned_b += 256;
                    }

                    const bool carry = (unsigned_a+unsigned_b)>256;
                    
                    int result = a+b;
                    if (result > 127) {
                        result -= 256;
                    } else if (result < -128) {
                        result += 256;
                    }

                    group_add.push_back(ComponentTestCase{ 
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{0, false}}, 
                            {"Increment", TestNumber{0, false}}, 
                            {"Propagate", TestNumber{1, false}}
                        }, 
                        {
                            {"Carry", TestNumber{carry, false}}, 
                            {"Data_out", TestNumber{result, true}}
                        } 
                    });
                }
            }
            test_groups.insert(make_pair("Add", group_add));

            vector<ComponentTestCase> group_sub;
            for (int a = -128; a < 128; a+=33) {
                for (int b = -128; b < 128; b+=33) {
                    int unsigned_a = a;
                    int unsigned_b = b;

                    if (unsigned_a < 0) {
                        unsigned_a += 256;
                    }
                    if (unsigned_b < 0) {
                        unsigned_b += 256;
                    }

                    const bool carry = (unsigned_a+unsigned_b)>256;

                    int result = a-b;
                    if (result > 127) {
                        result -= 256;
                    } else if (result < -128) {
                        result += 256;
                    }

                    group_sub.push_back(ComponentTestCase{ 
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{1, false}}, 
                            {"Increment", TestNumber{1, false}}, 
                            {"Propagate", TestNumber{1, false}}
                        },
                        {
                            {"Carry", TestNumber{carry, false}}, 
                            {"Data_out", TestNumber{result, true}}
                        } 
                    });
                }
            }
            test_groups.insert(make_pair("Subtract", group_sub));

            vector<ComponentTestCase> group_inc;
            for (int a = -128; a < 127; a+=33) {
                group_inc.push_back(ComponentTestCase{ 
                    {
                        {"Data-1_in", TestNumber{a, true}},
                        {"Data-2_in", TestNumber{0, false}}, 
                        {"Invert", TestNumber{0, false}}, 
                        {"Increment", TestNumber{1, false}}, 
                        {"Propagate", TestNumber{1, false}}
                    },
                    {
                        {"Carry", TestNumber{0, false}}, 
                        {"Data_out", TestNumber{a+1, true}}
                    } 
                });
            }
            test_groups.insert(make_pair("Increment", group_inc));

            vector<ComponentTestCase> group_dec;
            for (int a = -127; a < 128; a+=33) {
                group_dec.push_back(ComponentTestCase{ 
                    {
                        {"Data-1_in", TestNumber{a, true}}, 
                        {"Data-2_in", TestNumber{0, false}}, 
                        {"Invert", TestNumber{1, false}}, 
                        {"Increment", TestNumber{0, false}}, 
                        {"Propagate", TestNumber{1, false}}
                    },
                    {
                        {"Carry", TestNumber{0, false}}, 
                        {"Data_out", TestNumber{a-1, true}}
                    } 
                });
            }
            test_groups.insert(make_pair("Decrement", group_dec));

            vector<ComponentTestCase> group_inv;
            for (int b = -127; b < 128; b+=33) {
                group_inv.push_back(ComponentTestCase{ 
                    {
                        {"Data-1_in", TestNumber{0, false}}, 
                        {"Data-2_in", TestNumber{b, true}}, 
                        {"Invert", TestNumber{1, false}}, 
                        {"Increment", TestNumber{1, false}}, 
                        {"Propagate", TestNumber{1, false}}
                    },
                    {
                        {"Data_out", TestNumber{-b, true}}
                    } 
                });
            }
            test_groups.insert(make_pair("Invert", group_inv));

            vector<ComponentTestCase> group_not;
            for (int b = -127; b < 128; b+=33) {
                group_not.push_back(ComponentTestCase{ 
                    {
                        {"Data-1_in", TestNumber{0, false}}, 
                        {"Data-2_in", TestNumber{b, true}}, 
                        {"Invert", TestNumber{1, false}}, 
                        {"Increment", TestNumber{0, false}}, 
                        {"Propagate", TestNumber{0, false}}
                    },
                    {
                        {"Data_out", TestNumber{-b-1, true}}
                    } 
                });
            }
            test_groups.insert(make_pair("Not", group_not));

            vector<ComponentTestCase> group_xor;
            for (int a = -127; a < 128; a+=33) {
                for (int b = -127; b < 128; b+=33) {
                    group_xor.push_back(ComponentTestCase{ 
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{0, false}}, 
                            {"Increment", TestNumber{0, false}}, 
                            {"Propagate", TestNumber{0, false}}
                        },
                        {
                            {"Data_out", TestNumber{a^b, true}}
                        } 
                    });
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
            for (int a = -128; a < 128; a+=33) {
                for (int b = -128; b < 128; b+=33) {
                    group_and.push_back(ComponentTestCase{ 
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{0, false}}, 
                            {"Increment", TestNumber{0, false}}, 
                            {"Propagate", TestNumber{0, false}}
                        }, 
                        {
                            {"Data_out", TestNumber{a&b, true}}
                        } 
                    });
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
                        {
                            {"Data-1_in", TestNumber{a, true}}, 
                            {"Data-2_in", TestNumber{b, true}}, 
                            {"Invert", TestNumber{0, false}}, 
                            {"Increment", TestNumber{0, false}}, 
                            {"Propagate", TestNumber{0, false}}
                        }, 
                        {
                            {"Data_out", TestNumber{a|b, true}}
                        } 
                    });
                }
            }
            test_groups.insert(make_pair("Or", group_or));

            return ComponentTest(pin_blocks, test_groups);
        }
    }

    auto TestMemAdderAndRPC() -> void {
        Generate::MemAdderAndRPC().Run();
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