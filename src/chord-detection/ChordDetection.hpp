#ifndef CHORDDETECTION_HPP
#define CHORDDETECTION_HPP

#include <vector>
#include <string>

using namespace std;

class ChordDetection {
public:
    enum State {
        UNKNOWN,
        FIND_PEAK,
        FIND_TROUGH
    };

    vector<int> determine_peaks(const vector<float> &data, float threshold, float min_height, float min_width);

    float CalculateNoiseFloor(const vector<float> &data);

    string NoteLookup(float frequency);
};

#endif