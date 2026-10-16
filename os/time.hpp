#pragma once

#include <string>
#include <chrono>
#include <format>
using std::string;
namespace chrono = std::chrono;

namespace albert::time
{

inline long long getNowSeconds() {
    auto now = chrono::system_clock::now();
    auto seconds = chrono::floor<chrono::seconds>(now);
    return seconds.time_since_epoch().count();
}

inline string currentMinute() { //RETURNS [] FORMAT, USE IN LOGS ONLY
    long long s = getNowSeconds();
    chrono::seconds duration{s};
    chrono::sys_seconds tp{duration}; //a time point
    chrono::zoned_time local{chrono::current_zone(), tp};
    string human = std::format("{:%H:%M:%S}", local);
    return human;
}

}