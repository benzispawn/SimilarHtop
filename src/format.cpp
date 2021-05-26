#define HOUR 3600
#define MIN 60

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) { 
    int hour = time / HOUR;
    int temp = time % HOUR;
    int minute = temp / MIN;
    int second = temp % MIN;

    std::ostringstream final_time;
    final_time << std::setw(2) << std::setfill('0') << hour << ":"
               << std::setw(2) << std::setfill('0') << minute << ":"
               << std::setw(2) << std::setfill('0') << second;
    return final_time.str();
}