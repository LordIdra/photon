#include "Files.hpp"
#include "Util/Errors.hpp"
#include "Util/Util.hpp"

#include <fstream>
#include <iostream>
#include <filesystem>

using std::fstream;
using std::ofstream;



namespace Files {

    auto FilePaths(const string &path) -> vector<string> {
        if (!std::filesystem::exists(path)) {
            throw Errors::Files::NotFound(path);
        }

        vector<string> filenames;

        for (const auto &file : std::filesystem::directory_iterator{path}) {
            if (file.is_regular_file()) {
                filenames.push_back(file.path().filename());
            }
        }

        return filenames;
    }

    auto Read(const string &path) -> vector<string> {
        if (!std::filesystem::exists(path)) {
            throw Errors::Files::NotFound(path);
        }

        vector<string> lines;
        fstream file(path);
        string currentLine;

        while (std::getline(file, currentLine)) {
            lines.push_back(currentLine);
        }

        file.close();

        return lines;
    }

    auto Write(const string &path, const vector<string> &lines) -> void {
        ofstream file(path, std::ofstream::trunc);

        for (const string &line : lines) {
            file << line << "\n";
        }

        file.close();
    }
}