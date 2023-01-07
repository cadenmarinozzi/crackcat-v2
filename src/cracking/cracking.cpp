#include "../state/state.cpp"
#include "../hashes/SHA256.cpp"
#include "../IO/IO.cpp"
#include "../modules/utils.cpp"
#include <string>

using namespace std;
using namespace std::chrono;

void savePotfile(State *state)
{
    string potfileOutput = "";

    // Create the potfile output string
    for (auto const &[hash, password] : state->potfileCache)
    {
        // make sure the password is not already in the potfile
        if (state->potfile.find(password) != state->potfile.end())
        {
            continue;
        }

        potfileOutput += hash + ":" + password + "\n";
    }

    // Write the potfile cache to the potfile
    appendFile(state->potfilePath, potfileOutput);

    // Clear the potfile cache
    state->potfileCache.clear();
}

void crackLoop(State *state, string plainText)
{
    // Convert the password to a char array
    char plainTextCharArray[plainText.length() + 1];
    strcpy(plainTextCharArray, plainText.c_str());

    string hash = SHA256(plainTextCharArray);

    // Loop through the passlist
    for (int j = 0; j < state->passlist.size(); j++)
    {
        string password = state->passlist[j];

        if (state->hashPassList)
        {
            // Hash the password

            // Convert the plaintext password to a char array
            char passwordCharArray[password.length() + 1];
            strcpy(passwordCharArray, password.c_str());

            password = SHA256(passwordCharArray);
        }

        if (hash == password)
        {
            // Add the cracked password to the state
            if (state->keyPairOut)
            {
                state->crackedPairs[plainText] = password;
            }
            else
            {
                state->cracked.push_back(plainText);
            }

            if (state->verbose && !state->quiet)
            {
                // Print progress
                if (state->singleLineVerbose)
                {
                    if (state->printed)
                    {
                        // move up a line, go to the start of the line, clear the line
                        printf("\033[1A\r\033[K");
                    }
                    else
                    {
                        state->printed = true;
                    }
                }

                printf("%s : %s\n", plainText.c_str(), password.c_str()); // For some reason if the console is not wide enough, this line will not work
            }

            if (state->remove)
            {
                // Remove the password from the passlist
                // I don't know if this is the best way to do this
                state->passlist.erase(state->passlist.begin() + j);
                j--;
            }

            if (state->usePotfile)
            {
                // Add the cracked password to the potfile cache
                state->potfileCache[password] = plainText;

                // Check if the potfile should be written to
                milliseconds currentTime = getMilliseconds();
                milliseconds elapsedTime = currentTime - state->lastPotfileWrite;

                if (elapsedTime.count() / 1000 >= state->potfileDelay)
                {
                    savePotfile(state);

                    // Update the last potfile write time
                    state->lastPotfileWrite = getMilliseconds();
                }
            }
        }

        // Check if the max time has been reached
        if (state->maxTime != -1)
        {
            milliseconds currentTime = getMilliseconds();
            milliseconds elapsedTime = currentTime - state->startTime;

            if (elapsedTime.count() / 1000 >= state->maxTime)
            {
                return;
            }
        }
    }
}

void dictionaryAttack(State *state)
{
    if (state->multiThreaded)
    {
        if (!state->quiet)
        {
            printf("Multi-threading is not yet supported\n");
        }

        exit(1);
    }
    else
    {
        // Loop through the dictionary
        for (int i = 0; i < state->dictionary.size(); i++)
        {
            string dictionaryEntry = state->dictionary[i];

            crackLoop(state, dictionaryEntry);

            // Check if the max time has been reached
            if (state->maxTime != -1)
            {
                milliseconds currentTime = getMilliseconds();
                milliseconds elapsedTime = currentTime - state->startTime;

                if (elapsedTime.count() / 1000 >= state->maxTime)
                {
                    return;
                }
            }
        }
    }
}

// return the base 36 int representation of the string
// should return the same value as the javascript function parseInt(s, 36)
int parseInt(string s)
{
    int result = 0;
    int multiplier = 1;

    for (int i = s.length() - 1; i >= 0; i--)
    {
        char c = s[i];

        if (c >= '0' && c <= '9')
        {
            result += (c - '0') * multiplier;
        }
        else if (c >= 'a' && c <= 'z')
        {
            result += (c - 'a' + 10) * multiplier;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            result += (c - 'A' + 36) * multiplier;
        }

        multiplier *= 62;
    }

    return result;
}

void bruteforceAttack(State *state)
{
    string startChars = "aaaaa";
    string endChars = "ZZZZZ";
    int s = parseInt(startChars);
    int e = parseInt(endChars);

    for (int i = s; i <= e; i++)
    {
        string password = "";

        int j = i;
        while (j > 0)
        {
            int remainder = j % 62;

            if (remainder < 10)
            {
                password += (char)(remainder + '0');
            }
            else if (remainder < 36)
            {
                password += (char)(remainder - 10 + 'a');
            }
            else
            {
                password += (char)(remainder - 36 + 'A');
            }

            j /= 62;
        }

        crackLoop(state, password);

        // Check if the max time has been reached
        if (state->maxTime != -1)
        {
            milliseconds currentTime = getMilliseconds();
            milliseconds elapsedTime = currentTime - state->startTime;

            if (elapsedTime.count() / 1000 >= state->maxTime)
            {
                return;
            }
        }
    }
}

void beginAttack(State *state)
{
    state->lastPotfileWrite = getMilliseconds();

    // Loop through the passlist to check for potfile entries
    for (int j = 0; j < state->passlist.size(); j++)
    {
        string password = state->passlist[j];

        // Check if the password is in the potfile
        if (state->potfile.find(password) != state->potfile.end())
        {
            // Add the cracked password to the state
            state->cracked.push_back(state->potfile[password]);
            state->potfileCracked++;

            // Remove the password from the passlist
            state->passlist.erase(state->passlist.begin() + j);
            j--;
        }
    }

    if (state->attackMode == "dictionary")
    {
        dictionaryAttack(state);
    }
    else if (state->attackMode == "bruteforce")
    {
        bruteforceAttack(state);
    }
    else
    {
        if (!state->quiet)
        {
            printf("Attack mode %s not supported", state->attackMode.c_str());
        }

        exit(1);
    }

    if (state->verbose && !state->quiet && state->printed && state->singleLineVerbose)
    {
        // Clear the progress line
        printf("\033[1A\r\033[K");
    }

    // Save the potfile if it has not been saved yet
    if (state->usePotfile && state->potfileCache.size() > 0)
    {
        savePotfile(state);
    }
}