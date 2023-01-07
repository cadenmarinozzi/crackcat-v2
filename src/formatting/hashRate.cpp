#include <string>

using namespace std;

// Format hash rate to a human-readable string
// Add correct unit
string formatHashRate(int hashRate) {
    string unit;
    int rate;

    if (hashRate < 1000) {
        unit = "H/s";
        rate = hashRate;
    } else if (hashRate < 1000000) {
        unit = "kH/s";
        rate = (int) hashRate / 1000;
    } else if (hashRate < 1000000000) {
        unit = "MH/s";
        rate = (int) hashRate / 1000000;
    } else {
        unit = "GH/s";
        rate = (int) hashRate / 1000000000;
    }

    return to_string(rate) + " " + unit;
}