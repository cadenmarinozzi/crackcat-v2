#include "../state/state.cpp"
#include "../IO/IO.cpp"
#include "../formatting/time.cpp"
#include "../formatting/hashRate.cpp"
#include "../formatting/storage.cpp"
#include <string>

using namespace std;

// Not using string formatting cuz why not

string createHeader()
{
    string header;

    string version = "0.0.1";
    header += "crackcat v" + version + "\n";
    header += "\n";

    return header;
}

string createCracked(State state)
{
    string cracked;

    int deltaTime = state.endTime.count() - state.startTime.count();

    if (deltaTime < 1000)
    {
        deltaTime = 1000;
    }

    if (state.session != "")
    {
        cracked += "Session...........: " + state.session + "\n";
    }

    int nCracked;

    if (state.keyPairOut)
    {
        nCracked = state.crackedPairs.size();
    }
    else
    {
        nCracked = state.cracked.size();
    }

    cracked += "Start time........: " + timeToDate(state.startTime) + "\n";
    cracked += "End time..........: " + timeToDate(state.endTime) + "\n";
    cracked += "Time taken........: " + formatTime(state.endTime.count() - state.startTime.count()) + "\n";
    cracked += "Attack mode.......: " + state.attackMode + "\n";
    cracked += "Speed.............: " + to_string(nCracked / (deltaTime / 1000)) + " passwords per second\n";
    cracked += "Found.............: " + to_string(nCracked) + " passwords (" + to_string(nCracked / state.passlist.size() * 100) + "% of " + to_string(state.passlist.size()) + " passwords)\n";
    cracked += "Found in potfile..: " + to_string(state.potfileCracked) + " passwords\n";
    cracked += "Not found.........: " + to_string(state.passlist.size() - nCracked) + " passwords\n";
    cracked += "\n";

    return cracked;
}

string createBenchmark(State state)
{
    string benchmark;

    benchmark += "Hash rate: " + formatHashRate(state.hashRate) + "\n";
    benchmark += "Ideal estimated time to crack: " + to_string((int)round((double)state.dictionary.size() / state.hashRate)) + " seconds\n";
    benchmark += "\n";

    return benchmark;
}

string createSizeInfo(State state)
{
    string sizeInfo;

    sizeInfo += "Memory required: " +
                formatStorage((int)(state.dictionary.size() * sizeof(string)) + (int)(state.passlist.size() * sizeof(string))) +
                "\n";

    sizeInfo += "Dictionary size: " + to_string(state.dictionary.size()) + "\n";
    sizeInfo += "Passlist size: " + to_string(state.passlist.size()) + "\n";
    sizeInfo += "\n";

    return sizeInfo;
}

void writeLog(State state)
{
    string log;

    log += createHeader();
    log += createSizeInfo(state);
    log += createBenchmark(state);
    log += createCracked(state);

    string date = timeToDate(state.startTime);

    if (state.oneLog)
    {
        string logData = date + "\n" + log + "\n";

        appendFile(state.logPath, logData);

        return;
    }

    string logFile = "log-" + date + ".log";

    if (state.session != "")
    {
        logFile = state.session + "-" + logFile;
    }

    string logPath;

    if (state.logPath != "")
    {
        logPath = state.logPath;

        if (logPath.back() != '/')
            logPath += "/";
    }

    logPath += logFile;

    writeFile(logPath, log);
}