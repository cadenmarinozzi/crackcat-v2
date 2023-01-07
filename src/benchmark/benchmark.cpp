#include <string>
#include <vector>
#include <chrono>
#include "../hashes/SHA256.cpp"

using namespace std;

// Get hashes per second (hash rate)
int benchmarkHashing()
{
    typedef std::chrono::high_resolution_clock Clock;
    auto startTime = Clock::now();

    int nHashes = 0;

    while (true)
    {
        string hash = SHA256("Hello, world!");
        nHashes++;

        auto endTime = Clock::now();
        auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

        // Stop after 1 second
        if (timeElapsed >= 1)
        {
            break;
        }
    }

    return nHashes;
}