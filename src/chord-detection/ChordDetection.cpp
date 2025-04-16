#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>
#include <algorithm>
#include "ChordDetection.hpp"
#include <map>
#include <unordered_map>

using namespace std;

auto ChordDetection::DeterminePeaks(const vector<float> &data, float threshold,
                                    float min_height, float /*min_width*/)
    -> vector<int> {

  float mask_top = data[0];
  float mask_bottom = mask_top - min_height;

  float highest = data[0];
  int highest_index = 0;
  float lowest = data[0];

  vector<int> peak_indices;

  ChordDetection::State state = kUnknown;

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
      if (state == kUnknown || state == kFindPeak) {
        state = kFindTrough;
        if (highest > threshold) {
          peak_indices.push_back(highest_index);
        }
        lowest = mask_bottom;
      }
    }

    if (lowest < mask_bottom) {
      if (state == kUnknown || state == kFindTrough) {
        state = kFindPeak;
        highest = mask_top;
        highest_index = i;
      }
    }
  }

  return peak_indices;
}

auto ChordDetection::CalculateNoiseFloor(const vector<float> &data) -> float {
  float sum = 0.0;
  for (float value : data) {
    sum += value;
  }
  return sum / data.size();
}

auto ChordDetection::GetPeakFrequencies(vector<float> frequencies,
                                        const vector<float> &magnitudes)
    -> vector<float> {
  // Calculate the noise floor to adjust the threshold
  float noise_floor = CalculateNoiseFloor(magnitudes);
  float threshold = noise_floor * 30;

  // Run peak detection function
  std::vector<int> peak_indexes = DeterminePeaks(magnitudes, threshold, 0.5, 1);

  // Get frequencies of peaks
  std::vector<float> peak_frequencies;
  for (int peak_index : peak_indexes) {
    // Make sure freq is within bounds
    float freq = frequencies[peak_index];
    if (freq > 15) {
      peak_frequencies.push_back(frequencies[peak_index]);
    }
  }

  return peak_frequencies;
}

auto ChordDetection::NoteName(int note_num) -> string {
  vector<string> notes = {"A",  "A#", "B", "C",  "C#", "D",
                          "D#", "E",  "F", "F#", "G",  "G#"};

  int note_index = note_num % 12;

  if (note_index < 0) {
    note_index += 12;
  }
  return notes[note_index];
}

auto ChordDetection::NoteNumber(float frequency) -> int {
  int note_number = static_cast<int>(round(12 * log2(frequency / 440.0)));
  return note_number;
}

/* Chord Reference
 * Note:    1, m2, M2, m3, M3,  4, a4,  5, m6, M6, m7, M7,  8
 * Steps:   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
*/

map<vector<int>, string> triad_chord_table = {
    // Major
    // Notes:       1-3-5-8
    // Intervals:    4,3,5
    {{4,3,5}, "Major"},
    {{5,4,3}, "Major 1stInv"},
    {{3,5,4}, "Major 2ndInv"},
    {{4,3,5}, "Major"},
    {{2,2,3,5}, "Major"},
    // Minor
    // Notes:       1-m3-5-8
    // Intervals:    3,4,5
    {{3,4,5}, "Minor"},
    {{5,3,4}, "Minor 1stInv"},
    {{4,5,3}, "Minor 2ndInv"},
    {{2,1,4,5}, "Minor"},
    //{{2,1,4,3,2} "Minor"},
    // Diminished
    // Notes:       1-m3-m5-8
    // Intervals:    3,3,6
    {{3,3,6}, "Diminished"},
    // Augmented
    // Notes:       1-3-#5(m6)-8
    // Intervals:    4,4,4
    {{4,4,4}, "Augmented"},
    // Sus2
    // Notes:       1-2-5-8
    // Intervals:    2,5,5
    {{2,5,5}, "Sus2"},
    // Sus4
    // Notes:       1-4-5-8
    // Intervals:    5,2,5
    {{5,2,5}, "Sus4"},
    // Dominant 7
    // Notes:       1-3-5-m7-8
    // Intervals:    4,3,3,2
    {{4,3,3,2}, "7"},
    {{2,2,1,2,3,2}, "7"},
    {{2,2,1,2,3,1,1}, "7"},
    {{4,1,2,3,2}, "7"},
    // Major 7
    // Notes:       1-3-5-7-8
    // Intervals:    4,3,4,1
    {{4,3,4,1}, "Major 7"},
    {{4,2,1,4,1}, "Major 7"},
    {{2,2,2,1,4,1}, "Major 7"},
    // Minor 7
    // Notes:       1-m3-5-m7-8
    // Intervals:    3, 4,3, 2
    {{3,4,3,2}, "Minor 7"},
    {{2,1,2,2,3,2}, "Minor 7"},
    // Major 6
    // Notes:       1-3-5-6-8
    // Intervals:    4,3,2,3
    {{4,3,2,3}, "Major 6"},
    // Minor 6
    // Notes:       1-m3-5-m6-8
    // Intervals:    3, 4,1, 4
    {{3,4,1,4}, "Minor 6"}

};

auto NotesSetToChordType(vector<int> notes_set) -> string {
  // Print notes_set for debugging
  cout << "Notes set: ";
  for (int note : notes_set) {
    cout << note << " ";
  }
  cout << endl;

  // Get the intervals between each note.
  vector<int> intervals;
  int len = notes_set.size();
  for (size_t i = 1; i <= len; i++) {
    int interval = notes_set[i % len] - notes_set[i - 1];
    if (interval < 0)
      interval += 12;
    intervals.push_back(interval);
  }

  // Print intervals for debugging
  cout << "Intervals: ";
  for (int interval : intervals) {
    cout << interval << " ";
  }
  cout << endl;

  // match the chord type
  return triad_chord_table[intervals];
}

auto RemoveNonDuplicates(const vector<int> &vec) -> vector<int> {
  unordered_map<int, int> num_occurences;

  for (int num : vec) {
    num_occurences[num]++;
  }

  vector<int> new_vector;
  for (int num : vec) {
    if (num_occurences[num] > 1) {
      new_vector.push_back(num);
    }
  }

  return new_vector;
}

// string ChordDetection::ChordLookup(vector<int> notes, int root) {
auto ChordDetection::ChordLookup(const vector<float> &frequencies) -> string {

  // Get the note numbers from the peak freqeuncies
  std::vector<int> notes;
  notes.reserve(frequencies.size());
  for (float freq : frequencies) {
    notes.push_back(NoteNumber(freq));
  }

  // Check if there are any notes
  if (notes.empty()) {
    return "";
  }

  // Remove duplicates where the same freq has had 2 very close peaks
  notes.erase(unique(notes.begin(), notes.end()), notes.end());
  std::cout << "-----------------\nNotes going into algo\n------------------\n";
  for (int note : notes) {
    std::cout << "Number: " << note << " Note: " << NoteName(note) << std::endl;
  }

  // Find the root note
  int root = notes[0] % 12;
  if (root < 0)
    root += 12;

  // Strip ocatve information from notes
  vector<int> octaveless_notes = notes;

  for (int &octaveless_note : octaveless_notes) {
    int note_index = octaveless_note % 12;
    if (note_index < 0) {
      note_index += 12;
    }
    octaveless_note = note_index;
  }

  // Remove duplicates and sort
  vector<int> notes_set = octaveless_notes;
  sort(notes_set.begin(), notes_set.end());
  notes_set.erase(unique(notes_set.begin(), notes_set.end()), notes_set.end());

  // Rotate to put root note first
  auto root_it = find(notes_set.begin(), notes_set.end(), root);
  rotate(notes_set.begin(), root_it, notes_set.end());

  string chord_type = NotesSetToChordType(notes_set);
  // If couldn't find chord, remove root and try again
  if (chord_type.empty()) {
    std::cout << "Tryng next note as root\n";
    notes_set.erase(notes_set.begin());
    chord_type = NotesSetToChordType(notes_set);
  }
  // Next try removing any notes that only appear once
  if (chord_type.empty()) {
    std::cout << "Removing single notes\n";
    vector<int> notes_set = RemoveNonDuplicates(octaveless_notes);
    if (!notes_set.empty()) {
      root = notes_set[0];

      sort(notes_set.begin(), notes_set.end());
      notes_set.erase(unique(notes_set.begin(), notes_set.end()),
                      notes_set.end());

      auto root_it = find(notes_set.begin(), notes_set.end(), root);
      rotate(notes_set.begin(), root_it, notes_set.end());

      chord_type = NotesSetToChordType(notes_set);
    } else {
      chord_type = "";
    }
  }

  // Check for inversions and change root note
  if (chord_type == "Major 1stInv") {
    root += 5;
    chord_type = "Major";
  } else if (chord_type == "Minor 1stInv") {
    root += 5;
    chord_type = "Minor";
  } else if (chord_type == "Major 2ndInv") {
    root += 8;
    chord_type = "Major";
  } else if (chord_type == "Minor 2ndInv") {
    root += 9;
    chord_type = "Minor";
  }
  if (root >= 12) {
    root %= 12;
  }

  string root_note = ChordDetection::NoteName(root);
  string chord_name = root_note + " " + chord_type;
  return chord_name;
}
