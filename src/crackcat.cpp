#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include "IO/IO.cpp"
#include "state/state.cpp"
#include "benchmark/benchmark.cpp"
#include "cracking/cracking.cpp"
#include "terminal/terminal.cpp"
#include "modules/utils.cpp"

using namespace std;

string createOutputString(State state) {
    string outputString = "";

    if (state.keyPairOut) {
        for (auto const& pair : state.crackedPairs) {
            outputString += pair.first + ":" + pair.second + "\n";
        }
    } else {
        for (int i = 0; i < state.cracked.size(); i++) {
            string suffix = i == state.cracked.size() - 1 ? "" : "\n";
            outputString += state.cracked[i] + suffix;
        }
    }

    return outputString;
}

vector<string> removeDuplicates(vector<string> list) {
    vector<string> newList;
    unordered_map<string, bool> keys;

    for (int i = 0; i < list.size(); i++) {
        string key = list[i];

        if (keys.find(key) == keys.end()) {
            keys[key] = true;
            newList.push_back(key);
        }
    }

    return newList;
}

void optimizeDictionary(State *state) {
    if (state->removeDuplicates) {
        state->dictionary = removeDuplicates(state->dictionary);
    }
}


int main(int argc, char *argv[]) {   
    printHeader();
    
    // initialize state
    State state;
    initializeState(&state, argv);

    if (state.version) {
        return 0;
    }

    if (state.watermark) {
        printWatermark();
    }

    if (state.kitty) {
        printKitty();
    }

    printAuthor();

    if (state.attackMode == "dictionary") {
        optimizeDictionary(&state);
    }

    printSizeInfo(state);

    // benchmark hashing

    if (!state.disableBenchmark) {
        state.hashRate = benchmarkHashing();
        printBenchmark(state);
    }

    if (state.benchmarkOnly) {
        return 0;
    }

    // begin cracking
    state.startTime = getMilliseconds();
    beginAttack(&state);
    state.endTime = getMilliseconds();

    printCracked(state);

    // output cracked passwords
    string outputString = createOutputString(state);
    writeFile(state.outputPath, outputString);

    if (state.log) {
        writeLog(state);
    }

    return 0;
}