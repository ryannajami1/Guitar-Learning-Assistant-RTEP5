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

  // Lookup chord based on a vector of frequencies
  static auto ChordLookup(const vector<float> &frequencies) -> string;

  // Get peak frequencies from fft output
  static auto GetPeakFrequencies(vector<float> frequencies,
                          const vector<float> &magnitudes) -> vector<float>;
};

#endif