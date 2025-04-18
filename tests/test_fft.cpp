#include "../src/compute/guitarfft.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

// Function to generate a sine wave at a specific frequency
// Single sine wave as if an individual string was plucked
static void GenerateSineWave(std::vector<int16_t> &buffer,
                             float frequency, float amplitude,
                             float sample_rate) {
  for (size_t i = 0; i < buffer.size(); i++) {
    float const time = static_cast<float>(i) / sample_rate;
    double const value =
        amplitude * sin(2.0F * M_PI * static_cast<double>(frequency) *
                        static_cast<double>(time));
    buffer[i] = static_cast<int16_t>(value);
  }
}

// Function to generate a guitar chord (multiple frequencies)
// Simulates the input of a full chord with more than one string played
static void GenerateGuitarChord(std::vector<int16_t> &buffer,
                                const std::vector<float> &frequencies,
                                float amplitude, float sample_rate) {
  std::fill(buffer.begin(), buffer.end(), 0);

  // Temporary buffer for each frequency
  std::vector<int16_t> temp_buffer(buffer.size());

  // Add each frequency component
  for (const auto &freq : frequencies) {
    GenerateSineWave(temp_buffer, freq, amplitude / static_cast<float>(frequencies.size()),
                     sample_rate);

    // Sum into the main buffer
    for (size_t i = 0; i < buffer.size(); i++) {
      // Scale down to avoid overflow
      buffer[i] += temp_buffer[i];
    }
  }
}

// Test function that checks if the detected frequencies match the expected ones
// Confirms if the input signal correctly converts to the appropriate frequency
static auto
ValidateFrequencies(const std::vector<std::pair<float, float>> &detected_peaks,
                    const std::vector<float> &expected_frequencies,
                    float tolerance = 10.0F) -> bool {
  // Check that we detect at least one peak
  // Finds the dominent frequency
  if (detected_peaks.empty()) {
    std::cerr << "No frequency peaks detected!" << std::endl;
    return false;
  }

  // For each expected frequency, check if it was detected
  for (const auto &expected_freq : expected_frequencies) {
    bool found = false;

    // Look for a matching peak within tolerance
    // Tolerance accounts for likely sub-optimal tuning of the guitar
    // This ensures that the frequency can be identified even if not exactly 100%
    for (const auto &peak : detected_peaks) {
      if (std::abs(peak.first - expected_freq) <= tolerance) {
        found = true;
        std::cout << "Expected frequency " << expected_freq
                  << " Hz matched with detected " << peak.first
                  << " Hz (magnitude: " << peak.second << ")" << std::endl;
        break;
      }
    }

    if (!found) {
      std::cerr << "Failed to detect expected frequency: " << expected_freq
                << " Hz" << std::endl;
      return false;
    }
  }

  return true;
}

auto main() -> int {
    // Define constants for frame size
    const unsigned int FRAME_SIZE = 1024; // Assuming a reasonable frame size
    const unsigned int FRAMES_TO_PROCESS = 4; // Number of frames to process at once
    
    // Sample rate
    // Identical to guitarfft.cpp
    const float sample_rate = 2000.0F;

    // Initialize FFT processor
    GuitarFFTProcessor processor(FRAME_SIZE, static_cast<unsigned int>(sample_rate), FRAMES_TO_PROCESS);
    if (!processor.Initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    // Simple E major chord (E, G#, B)
    // Frequencies that match the chord
    std::vector<float> e_chord_frequencies = {
        82.41F,  // E2
        164.81F, // E3 (first harmonic)
        207.65F, // G#3
        246.94F  // B3
    };

    std::cout << "Testing E major chord detection..." << std::endl;
    
    // Create buffer for the simulated chord - size is now FRAME_SIZE * FRAMES_TO_PROCESS
    const size_t total_samples = FRAME_SIZE * FRAMES_TO_PROCESS;
    std::vector<int16_t> buffer(total_samples);
    
    // Generate the chord data directly into the vector
    GenerateGuitarChord(buffer, e_chord_frequencies, 16000.0F, sample_rate);
    
    // Process the buffer in one go
    processor.ProcessFrames(buffer);
    
    // Get the detected peaks
    auto detected_peaks = processor.GetFrequencyPeaks();
    
    // Validate the results
    bool test_passed = ValidateFrequencies(detected_peaks, e_chord_frequencies);
    
    // Clean up
    processor.Cleanup();
    
    return test_passed ? 0 : 1;
}