#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "../IO/IO.cpp"
#include <regex>
#include "../modules/utils.cpp"

using namespace std;
using namespace std::chrono;

struct State
{
    string dictionaryPath;
    string passlistPath;
    string outputPath;

    int cutoff = 0;
    int skip = 0;

    int maxTime = 0;
    int nThreads = 0;

    bool hashPassList = false;

    string attackMode;
    string hashType;

    bool log = false;
    string logPath;
    bool oneLog = false;

    int potfileCracked = 0;

    bool multiThreaded = false;

    bool usePotfile = false;
    string potfilePath;
    int potfileDelay = 0;

    vector<string> dictionary;
    vector<string> passlist;
    vector<string> cracked;

    vector<unordered_map<string, unordered_map<string, string>>> bucketPassslist;
    vector<unordered_map<string, unordered_map<string, string>>> bucketCracked;

    bool printed = false;

    milliseconds lastPotfileWrite;

    unordered_map<string, string> crackedPairs;

    milliseconds startTime;
    milliseconds endTime;

    bool version = false;
    bool quiet = false;
    bool verbose = false;

    bool force = false;

    bool remove = false;

    bool watermark = false;
    bool kitty = false;

    bool keyPairOut = false;

    bool disableBenchmark = false;
    bool benchmarkOnly = false;

    int hashRate = false;

    bool singleLineVerbose = false;

    bool removeDuplicates = false;

    string session;

    unordered_map<string, string> potfileCache;
    unordered_map<string, string> potfile;

    string lineFormat;
    bool useLineFormat = false;
};

// Get information from a line in the format of state->lineFormat
// Example: state->lineFormat = "email||:||user:***pass||:||###salt" should get the email, user, and pass, and salt
// Rules: *** means the rest of the line is the password
// ### means the rest of the line is the salt
// || means the next character is a delimiter
unordered_map<string, string> getLineFromFormat(State *state, string line)
{
    smatch delimeterMatch;
    regex delimeterMatchRegex = regex("\\|\\|.\\|\\|");
    regex_search(state->lineFormat, delimeterMatch, delimeterMatchRegex);

    string delimeter = delimeterMatch[0];
    regex delimeterRegex = regex("\\|\\|");
    delimeter = regex_replace(delimeter, delimeterRegex, "");

    vector<string> lineFormatParts = split(state->lineFormat, delimeter);
    vector<string> lineParts = split(line, delimeter);

    unordered_map<string, string> lineMap;

    for (int i = 0; i < lineFormatParts.size(); i++)
    {
        string lineFormatPart = lineFormatParts[i];
        string linePart = lineParts[i];

        printf("%s:%s\n", lineFormatPart.c_str(), linePart.c_str());

        lineMap[lineFormatPart] = linePart;
    }

    return lineMap;
}

void readFileLinesWithFormat(State *state, string fileName)
{
    ifstream file = handleOpenFile(fileName);

    string line;

    getline(file, line);
    getLineFromFormat(state, line);

    file.close();
}

void initializeState(State *state, char *argv[])
{
    state->dictionaryPath = argv[1];
    state->passlistPath = argv[2];
    state->outputPath = argv[3];

    state->cutoff = atoi(argv[4]);

    state->maxTime = atoi(argv[5]); // seconds

    state->multiThreaded = false;
    state->nThreads = 4;

    state->keyPairOut = true;

    state->usePotfile = false;
    state->potfilePath = "potfile.txt";
    state->potfileDelay = 1; // seconds

    state->remove = true;

    state->watermark = true;
    state->kitty = false;

    state->attackMode = "dictionary";
    state->hashType = "sha256";

    state->singleLineVerbose = true;

    state->verbose = true;
    state->quiet = false;
    state->version = false;

    state->force = false;

    state->disableBenchmark = false;

    state->log = true;
    state->oneLog = true;
    state->logPath = "log.txt";

    state->session = "crackcat";

    state->hashPassList = false;

    state->removeDuplicates = true;

    state->lineFormat = "email||:||***password";
    state->useLineFormat = true;

    state->dictionary = readFileLines(state->dictionaryPath, state->cutoff);

    if (state->useLineFormat)
    {
        readFileLinesWithFormat(state, state->passlistPath);
    }
    else
    {
        state->passlist = readFileLines(state->passlistPath);
    }

    if (state->usePotfile && fileExists(state->potfilePath))
    {
        state->potfile = readPotfile(state->potfilePath);
    }
}

#endif