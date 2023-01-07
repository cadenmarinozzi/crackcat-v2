#include <string>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

string timeToDate(milliseconds ms) {
    seconds s = duration_cast<seconds>(ms);
    time_t t = s.count();
    tm* time = localtime(&t);

    return to_string(time->tm_year + 1900) + "-" + to_string(time->tm_mon + 1) + "-" + to_string(time->tm_mday) + " " + to_string(time->tm_hour) + ":" + to_string(time->tm_min) + ":" + to_string(time->tm_sec);
}

// Format time a human-readable string
// Add correct unit
string formatTime(int milliseconds) {
    string unit;
    int time;

    if (milliseconds < 1000) {
        unit = "ms";
        time = milliseconds;
    } else if (milliseconds < 60000) {
        unit = "s";
        time = milliseconds / 1000;
    } else if (milliseconds < 3600000) {
        unit = "m";
        time = milliseconds / 60000;
    } else {
        unit = "h";
        time = milliseconds / 3600000;
    }

    return to_string(time) + unit;
}