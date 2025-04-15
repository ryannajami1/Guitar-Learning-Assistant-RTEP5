#include "../src/compute/guitarfft.hpp"
#include <cmath>
#include <vector>
#include <array>

// Function to generate a sine wave at a specific frequency
void GenerateSineWave(std::array<int16_t, FRAMES> &buffer, float frequency,
                      float amplitude, float sample_rate) {
  for (size_t i = 0; i < buffer.size(); i++) {
    float time = static_cast<float>(i) / sample_rate;
    float value = amplitude * sin(2.0F * M_PI * frequency * time);
    buffer[i] = static_cast<int16_t>(value);
  }
}

// Function to generate a guitar chord (multiple frequencies)
void GenerateGuitarChord(std::array<int16_t, FRAMES> &buffer,
                         const std::vector<float> &frequencies, float amplitude,
                         float sample_rate) {
  std::fill(buffer.begin(), buffer.end(), 0);

  // Temporary buffer for each frequency
  std::array<int16_t, FRAMES> temp_buffer;

  // Add each frequency component
  for (const auto &freq : frequencies) {
    GenerateSineWave(temp_buffer, freq, amplitude / frequencies.size(),
                     sample_rate);

    // Sum into the main buffer
    for (size_t i = 0; i < buffer.size(); i++) {
      // Scale down to avoid overflow
      buffer[i] += temp_buffer[i];
    }
  }
}

// Test function that checks if the detected frequencies match the expected ones
auto ValidateFrequencies(
    const std::vector<std::pair<float, float>> &detected_peaks,
    const std::vector<float> &expected_frequencies, float tolerance = 10.0F)
    -> bool {
  // Check that we detect at least one peak
  if (detected_peaks.empty()) {
    std::cerr << "No frequency peaks detected!" << std::endl;
    return false;
  }

  // For each expected frequency, check if it was detected
  for (const auto &expected_freq : expected_frequencies) {
    bool found = false;

    // Look for a matching peak within tolerance
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
    // Sample rate used by the FFT processor
    const float sample_rate = 2000.0F;

    // Initialize FFT processor
    GuitarFFTProcessor processor(FRAMES, sample_rate, FRAMES);
    if (!processor.Initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    // Test with a simple E major chord (E, G#, B)
    std::vector<float> e_chord_frequencies = {
        82.41F,  // E2
        164.81F, // E3 (first harmonic)
        207.65F, // G#3
        246.94F  // B3
    };

    std::cout << "Testing E major chord detection..." << std::endl;
    
    // Create buffer for the simulated chord
    std::array<int16_t, FRAMES> buffer;
    
    // Generate enough frames to trigger FFT processing
    bool test_passed = true;
    for (unsigned int i = 0; i < processor.GetFramesNeeded(); i++) {
        // Generate a frame with the E chord frequencies
        GenerateGuitarChord(buffer, e_chord_frequencies, 16000.0F, sample_rate);

        // Add the frame to the processor
        processor.AddFrame(buffer);
    }
    
    // Process the last batch if needed
    processor.ProcessFrames();
    
    // Get the detected peaks
    auto detected_peaks = processor.GetFrequencyPeaks();
    
    // Validate the results
    if (!ValidateFrequencies(detected_peaks, e_chord_frequencies)) {
      test_passed = false;
    }

    // Clean up
    processor.Cleanup();
    
    return test_passed ? 0 : 1;
}
