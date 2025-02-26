#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>
#include "ChordDetection.hpp"

using namespace std;

vector<int> ChordDetection::determine_peaks(const vector<float> &data, float threshold, float min_height, float min_width) {

    float mask_top = data[0];
    float mask_bottom = mask_top - min_height;

    float highest = data[0];
    int highest_index = 0;
    float lowest = data[0];

    vector<int> peak_indices;

    ChordDetection::State state = UNKNOWN;

    for (size_t i = 0; i < data.size(); i++) {
        // move mask
        if (data[i] > mask_top) {
            mask_top = data[i];
            mask_bottom = mask_top - min_height;
            highest = data[i];
            highest_index = i;
        } else if (data[i] < mask_bottom) {
            mask_bottom = data[i];
            mask_top = mask_bottom + min_height;
            lowest = data[i];
        }

        // Check if mask has shifted away from peak or trough
        if (highest > mask_top) {
            if (state == UNKNOWN || state == FIND_PEAK) {
                state = FIND_TROUGH;
                if (highest > threshold) {
                    peak_indices.push_back(highest_index);
                }
                lowest = mask_bottom;
            }
        }

        if (lowest < mask_bottom) {
            if (state == UNKNOWN || state == FIND_TROUGH) {
                state = FIND_PEAK;
                highest = mask_top;
                highest_index = i;
            }
        }
    }

    return peak_indices;
}

float ChordDetection::CalculateNoiseFloor(const vector<float> &data) {
    float sum = 0.0;
    for (float value : data) {
        sum += value;
    }
    return sum / data.size();
}

string ChordDetection::NoteLookup(float frequency) {
    vector<string> notes = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

    int note_index = static_cast<int>(round(12 * log2(frequency / 440.0))) % 12;

    if (note_index < 0) {
        note_index += 12;
    }
    return notes[note_index];
}