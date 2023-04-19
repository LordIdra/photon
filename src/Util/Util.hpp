#pragma once

#include <Util/Types.hpp>



auto Pow(int x, const int y) -> int;

auto UnsignedBinaryToDenary(const vector<bool> &bits) -> int;
auto SignedBinaryToDenary(const vector<bool> &bits) -> int;

auto SignedDenaryToBinary(int x, const int bits) -> vector<bool>;
auto UnsignedDenaryToBinary(int x, const int bits) -> vector<bool>;
auto UnsignedDenaryToBinaryString(int x, const int bits) -> string;

auto Contains(const vector<int> container, const int target) -> bool;

auto FormatValue(int x) -> string;
auto FormatAddress(int x) -> string;