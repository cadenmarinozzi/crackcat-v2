#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

milliseconds getMilliseconds() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));

    return res;
}

#endif