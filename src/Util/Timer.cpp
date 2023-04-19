#include "Timer.hpp"
#include <chrono>



auto Timer::Start() -> void {
    begin = steady_clock::now();
}

auto Timer::Stop() -> void {
    end = steady_clock::now();
}

auto Timer::GetMilliseconds() -> int {
    return std::chrono::duration_cast<milliseconds>(end - begin).count();
}