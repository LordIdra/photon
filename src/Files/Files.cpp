#include "Files.hpp"
#include "Util/Errors.hpp"
#include "Util/Util.hpp"

#include <fstream>
#include <iostream>

using std::ifstream;



namespace Files {
    auto Read(const string &path) -> vector<string> {
        if (!FileExists(path)) {
            throw Errors::Files::NotFound(path);
        }
        vector<string> lines;
        ifstream file(path);
        string currentLine;
        while (std::getline(file, currentLine)) {
            lines.push_back(currentLine);
        }
        return lines;
    }
}