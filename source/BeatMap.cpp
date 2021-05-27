#include "BeatMap.hpp"
#include "debug.hpp"

#include <3ds.h>
#include <limits>

BeatMap::BeatMap(std::ifstream& a_file)
{
    if (!a_file.is_open()) {
        eprintf("PANIC: BTS file stream received is not open!\n");
        svcBreak(USERBREAK_PANIC);
    }

    long currentBeat;
    while (a_file >> currentBeat) {
        beats.push_back(currentBeat);
        // ignore the rest of the line
        a_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    eprintf("Loaded %d beats from BTS file\n", beats.size());
};

long BeatMap::peekBeat()
{
    if (beats.empty()) {
        return -1;
    }

    return beats.front();
};

long BeatMap::popBeat()
{
    if (beats.empty()) {
        return -1;
    }

    long ret = beats.front();
    beats.pop_front();

    return ret;
};