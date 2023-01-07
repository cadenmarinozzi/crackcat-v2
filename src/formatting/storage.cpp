#include <string>

using namespace std;

// Format storage a human-readable string
// Add correct unit
string formatStorage(int bytes) {
    string unit;
    int size;

    if (bytes < 1000) {
        unit = "B";
        size = bytes;
    } else if (bytes < 1000000) {
        unit = "kB";
        size = (int) bytes / 1000;
    } else if (bytes < 1000000000) {
        unit = "MB";
        size = (int) bytes / 1000000;
    } else {
        unit = "GB";
        size = (int) bytes / 1000000000;
    }

    return to_string(size) + " " + unit;
}