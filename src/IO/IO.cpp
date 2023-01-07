#ifndef IO_H
#define IO_H

#include <fstream>
#include <string>
#include <vector>
#include "../modules/utils.cpp"
#include <unordered_map>

using namespace std;

ifstream handleOpenFile(string fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        printf("Error opening file \"%s\"\n", fileName.c_str());
        exit(1);
    }

    return file;
}

ofstream handleOpenWriteFile(string fileName)
{
    ofstream file(fileName);

    if (!file.is_open())
    {
        printf("Error opening file \"%s\"\n", fileName.c_str());
        exit(1);
    }

    return file;
}

ofstream handleOpenAppendFile(string fileName)
{
    ofstream file(fileName, ios::app);

    if (!file.is_open())
    {
        printf("Error opening file \"%s\"\n", fileName.c_str());
        exit(1);
    }

    return file;
}

vector<string> readFileLines(string fileName, int cutoff = -1)
{
    ifstream file = handleOpenFile(fileName);

    string line;
    vector<string> lines;

    while (getline(file, line) && (cutoff == -1 || lines.size() < cutoff))
    {
        lines.push_back(line);
    }

    file.close();

    return lines;
}

void writeFile(string fileName, string data)
{
    ofstream file = handleOpenWriteFile(fileName);

    file << data;

    file.close();
}

void appendFile(string fileName, string data)
{
    ofstream file = handleOpenAppendFile(fileName);

    file << data;

    file.close();
}

bool fileExists(string fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        return false;
    }

    file.close();

    return true;
}

unordered_map<string, string> readPotfile(string potfilePath)
{
    unordered_map<string, string> potfile;

    vector<string> lines = readFileLines(potfilePath);

    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];

        if (line == "" || line == "\n")
        {
            continue;
        }

        string seperator = ":";
        vector<string> parts = split(line, seperator);

        string hash = parts[0];
        string password = parts[1];

        potfile[hash] = password;
    }

    return potfile;
}

#endif