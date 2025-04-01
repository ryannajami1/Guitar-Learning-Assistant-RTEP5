#include <iostream>
#include "../src/chord-detection/ChordDetection.hpp"
#include <map>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

int test_determine_peaks() {
    ChordDetection cd;

    // Test the peak detection

    // Generate sin wave data
    vector<float> wave;

    for (int i = 0; i < 100; i++) {
        wave.push_back(sin(i * 3.141592 / 20));
    }

    // Run function
    vector<int> peaks = cd.determine_peaks(wave, 0.5, 0.5, 0);

    // Check indexes are correct
    for (int index : peaks) {
        if ((index + 10) % 20 != 0) {
            cout << "Incorrect peak detected. Test failed.\n";
            cout << "Index: " << index << "\n";
            return 1;
        }
    }

    cout << "Peak Detection test passed!\n";

    return 0;
}

int test_note_lookup() {
    ChordDetection cd;

    // Test the note lookup ------------------------------------------------------------------------------------------

    map<string, float> noteFrequencies = {
        {"C", 16.35}, {"C#", 17.32}, {"D", 18.35}, {"D#", 19.45}, {"E", 20.60}, {"F", 21.83}, {"F#", 23.12}, {"G", 24.50}, {"G#", 25.96}, {"A", 27.50}, {"A#", 29.14}, {"B", 30.87},
        {"C", 32.70}, {"C#", 34.65}, {"D", 36.71}, {"D#", 38.89}, {"E", 41.20}, {"F", 43.65}, {"F#", 46.25}, {"G", 49.00}, {"G#", 51.91}, {"A", 55.00}, {"A#", 58.27}, {"B", 61.74},
        {"C", 65.41}, {"C#", 69.30}, {"D", 73.42}, {"D#", 77.78}, {"E", 82.41}, {"F", 87.31}, {"F#", 92.50}, {"G", 98.00}, {"G#", 103.83}, {"A", 110.00}, {"A#", 116.54}, {"B", 123.47},
        {"C", 130.81}, {"C#", 138.59}, {"D", 146.83}, {"D#", 155.56}, {"E", 164.81}, {"F", 174.61}, {"F#", 185.00}, {"G", 196.00}, {"G#", 207.65}, {"A", 220.00}, {"A#", 233.08}, {"B", 246.94},
        {"C", 261.63}, {"C#", 277.18}, {"D", 293.66}, {"D#", 311.13}, {"E", 329.63}, {"F", 349.23}, {"F#", 369.99}, {"G", 392.00}, {"G#", 415.30}, {"A", 440.00}, {"A#", 466.16}, {"B", 493.88},
        {"C", 523.25}, {"C#", 554.37}, {"D", 587.33}, {"D#", 622.25}, {"E", 659.25}, {"F", 698.46}, {"F#", 739.99}, {"G", 783.99}, {"G#", 830.61}, {"A", 880.00}, {"A#", 932.33}, {"B", 987.77},
        {"C", 1046.50}, {"C#", 1108.73}, {"D", 1174.66}, {"D#", 1244.51}, {"E", 1318.51}, {"F", 1396.91}, {"F#", 1479.98}, {"G", 1567.98}, {"G#", 1661.22}, {"A", 1760.00}, {"A#", 1864.66}, {"B", 1975.53},
        {"C", 2093.00}, {"C#", 2217.46}, {"D", 2349.32}, {"D#", 2489.02}, {"E", 2637.02}, {"F", 2793.83}, {"F#", 2959.96}, {"G", 3135.96}, {"G#", 3322.44}, {"A", 3520.00}, {"A#", 3729.31}, {"B", 3951.07},
        {"C", 4186.01}, {"C#", 4434.92}, {"D", 4698.63}, {"D#", 4978.03}, {"E", 5274.04}, {"F", 5587.65}, {"F#", 5919.91}, {"G", 6271.93}, {"G#", 6644.88}, {"A", 7040.00}, {"A#", 7458.62}, {"B", 7902.13}
    };

    for (const auto& note : noteFrequencies) {
        string note_name = cd.NoteName(cd.NoteNumber(note.second));
        if (note_name != note.first) {
            cout << note.first << " | " << note_name;
            cerr << "Note Lookup failed :(((\n";
            return 1;
        }
    }

    cout << "Note Lookup test passed!\n";
    
    return 0;

}

int test_fft_data() {

    ChordDetection cd;

    // Read FFT data from CSV file
    ifstream file("../../tests/fft-data/scope_4_fft.csv");
    if (!file.is_open()) {
        cerr << "Failed to open FFT data file.\n";
        return 1;
    }

    cout << "File opened succefully!\n";

    vector<vector<float>> fft_data;
    string line;

    // Remove header
    getline(file, line);

    // Read in csv data
    while (getline(file, line)) {
        vector<float> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            row.push_back(stof(cell));
        }
        fft_data.push_back(row);
    }

    file.close();

    // Get the magnitude column 
    vector<float> fft_magnitude;
    for (vector<float> row : fft_data) {
        fft_magnitude.push_back(row[1]);
        cout << row[1] << "\n";
    }

    // Find the noise floor
    float noise_floor = cd.CalculateNoiseFloor(fft_magnitude);
    float threshold = noise_floor * 30;
    cout << "threshold " << threshold << "\n";

    // Run FFT processing function
    vector<int> peaks = cd.determine_peaks(fft_magnitude, threshold, 0.1, 0);

    // Get frequencies of peaks
    vector<float> frequencies;
    for (int peak_index : peaks) {
        float f = fft_data[peak_index][0];
        cout << f << "\n";
        frequencies.push_back(fft_data[peak_index][0]);
    }

    cout << "FFT Data test passed!\n";
    return 0;
}

auto main(int, char**) -> int {

    if (test_determine_peaks()) return 1;
    if (test_note_lookup()) return 1;
    if (test_fft_data()) return 1;
    
    return 0;
}