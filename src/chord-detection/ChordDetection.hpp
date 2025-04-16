#ifndef CHORDDETECTION_HPP
#define CHORDDETECTION_HPP

#include <vector>
#include <string>

using namespace std;

class ChordDetection {
public:
  enum State { kUnknown, kFindPeak, kFindTrough };

  /* Terms used
   * ==========
   * Note Name:   Letter of note plus accident
   * Note Number: Distance of note from A4 (440Hz) in half steps
   * Note Index:  Note number ignoring the octaves. 0 is A and 11 is G#
   */

  // Takes in a vector representing the signal and returns the indexes of all
  // the peaks.
  static auto DeterminePeaks(const vector<float> &data, float threshold,
                             float min_height, float min_width) -> vector<int>;

  // Calculates the noise floor of signal (finds mean)
  static auto CalculateNoiseFloor(const vector<float> &data) -> float;

  // Lookup note name from note number. Ignores octave
  static auto NoteName(int note_num) -> string;

  // Calculate note number from frequency.
  static auto NoteNumber(float frequency) -> int;

  // Lookup chord based on a list of note numbers
  auto ChordLookup(const vector<float> &frequencies) -> string;

  // Get peak frequencies from fft output
  auto GetPeakFrequencies(vector<float> frequencies,
                          const vector<float> &magnitudes) -> vector<float>;
};

#endif