#define HOUR 3600
#define MIN 60

#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) { 
    int hour = time / HOUR;
    int minute = (time / MIN) % MIN;
    int second = time % MIN;
    return std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
}

// string Format::prd(const double x, const int decDigits) {
//     std::stringstream ss = "";
//     ss << fixed;
//     ss.precision(decDigits); // set # places after decimal
//     ss << x;
//     return ss.str();
// }