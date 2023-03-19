#include "CommandLine/CommandLine.hpp"

#include "Util/Util.hpp"



auto main(const int argument_count, char* argument_array[]) -> int {
    vector<string> arguments;
    arguments.reserve(argument_count);
    for (int i = 1; i < argument_count; i++) {
        arguments.emplace_back(argument_array[i]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    CommandLine::Command(arguments);
}