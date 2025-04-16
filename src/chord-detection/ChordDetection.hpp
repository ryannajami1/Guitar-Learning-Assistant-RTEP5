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

    /* Terms used
     * ==========
     * Note Name:   Letter of note plus accident
     * Note Number: Distance of note from A4 (440Hz) in half steps
     * Note Index:  Note number ignoring the octaves. 0 is A and 11 is G#
    */

    // Takes in a vector representing the signal and returns the indexes of all the peaks.
    vector<int> determine_peaks(const vector<float> &data, float threshold, float min_height, float min_width);

    // Calculates the noise floor of signal (finds mean)
    float CalculateNoiseFloor(const vector<float> &data);

    // Lookup note name from note number. Ignores octave
    string NoteName(int note_num);

    // Calculate note number from frequency.
    int NoteNumber(float frequency);

    // Lookup chord based on a list of note numbers
    string ChordLookup(vector<float> frequencies);

    // Get peak frequencies from fft output
    vector<float> GetPeakFrequencies(vector<float> frequencies, vector<float> magnitudes);
};

#endif