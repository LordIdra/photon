#include <chrono>

using std::chrono::time_point;
using std::chrono::steady_clock;
using std::chrono::milliseconds;



class Timer {
private:
    time_point<steady_clock> begin;
    time_point<steady_clock> end;

public:
    auto Start() -> void;
    auto Stop() -> void;
    auto GetMilliseconds() -> int;
};