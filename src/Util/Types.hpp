#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <set>
#include <unordered_map>
#include <memory>
#include <optional>
#include <filesystem>



using std::string;
using std::vector;
using std::stack;
using std::array;
using std::list;
using std::set;
using std::unordered_map;
using std::unique_ptr;
using std::shared_ptr;
using std::optional;
using std::pair;

using std::to_string;
using std::cout;
using std::pair;
using std::make_pair;
using std::make_optional;

const int PROPAGATION_DELAY_MICROSECONDS = 200;
const int READ_DELAY_MICROSECONDS = 140;
const int WRITE_DELAY_MICROSECONDS = 140;