#pragma once

#include <Util/Types.hpp>



namespace Files {
    auto FilePaths(const string &path) -> vector<string>;
    auto Read(const string &path) -> vector<string>;
    auto Write(const string &path, const vector<string> &lines) -> void;
}