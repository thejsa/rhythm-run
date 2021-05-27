/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once

#include <deque>
#include <fstream>
#include <iostream>

class BeatMap {
public:
    float bpm = 150;
    std::deque<long> beats;

    BeatMap() {};
    BeatMap(std::ifstream& a_file);

    long peekBeat();
    long popBeat();
};