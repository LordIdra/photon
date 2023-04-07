#pragma once

#include <Util/Types.hpp>



auto Pow(int x, const int y) -> int;
auto DenaryToBinary(int x, const int bits) -> vector<bool>;
auto DenaryToBinaryString(int x, const int bits) -> string;
auto Contains(const vector<int> container, const int target) -> bool;

auto FormatValue(int x) -> string;
auto FormatAddress(int x) -> string;