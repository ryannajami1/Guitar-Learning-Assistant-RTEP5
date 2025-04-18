#include "../src/compute/guitarfft.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

// Define a mock for the websocket function to avoid linking errors
// This will replace the actual LWS_SendMessage function during testing
extern "C" {
    void LWS_SendMessage(const std::string& message) {
        std::cout << "MOCK: Would send websocket message: " << message << std::endl;
    }
}

// Function to generate a sine wave at a specific frequency
// As if a sinlge string was played
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
// Full chord with more than one string
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
      // Fixed: Prevent integer overflow by using a temporary int32_t
      int32_t sum = static_cast<int32_t>(buffer[i]) + static_cast<int32_t>(temp_buffer[i]);
      // Clamp the value to int16_t range
      buffer[i] = static_cast<int16_t>(std::min(static_cast<int32_t>(INT16_MAX), 
                                     std::max(static_cast<int32_t>(INT16_MIN), sum)));
    }
  }
}

// Test function that checks if the detected frequencies match the expected ones
static auto
ValidateFrequencies(const std::vector<std::pair<float, float>> &detected_peaks,
                    const std::vector<float> &expected_frequencies,
                    float tolerance = 10.0F) -> bool {
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
    // Define constants for frame size
    const unsigned int FRAME_SIZE = 1024; // Assuming a reasonable frame size
    const unsigned int FRAMES_TO_PROCESS = 4; // Number of frames to process at once
    
    // Sample rate
    const float sample_rate = 2000.0F;

    std::cout << "Initializing FFT processor..." << std::endl;
    // Initialize FFT processor
    GuitarFFTProcessor processor(FRAME_SIZE, static_cast<unsigned int>(sample_rate), FRAMES_TO_PROCESS);
    if (!processor.Initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    // Simple E major chord (E, G#, B)
    std::vector<float> e_chord_frequencies = {
        82.41F,  // E2
        164.81F, // E3 (first harmonic)
        207.65F, // G#3
        246.94F  // B3
    };

    std::cout << "Testing E major chord detection..." << std::endl;
    
    // Create buffer for the simulated chord
    // Fixed: Use explicit cast to size_t to avoid implicit widening conversion warning
    const size_t total_samples = static_cast<size_t>(FRAME_SIZE) * static_cast<size_t>(FRAMES_TO_PROCESS);
    std::vector<int16_t> buffer(total_samples);
    
    // Generate the chord data directly into the vector
    std::cout << "Generating test chord..." << std::endl;
    GenerateGuitarChord(buffer, e_chord_frequencies, 16000.0F, sample_rate);
    
    // Process the buffer in one go
    std::cout << "Processing audio frames..." << std::endl;
    processor.ProcessFrames(buffer);
    
    // Get the detected peaks
    std::cout << "Getting frequency peaks..." << std::endl;
    auto detected_peaks = processor.GetFrequencyPeaks();
    
    // Validate the results
    std::cout << "Validating results..." << std::endl;
    bool test_passed = ValidateFrequencies(detected_peaks, e_chord_frequencies);
    
    // Clean up
    std::cout << "Cleaning up..." << std::endl;
    processor.Cleanup();
    
    if (test_passed) {
        std::cout << "✓ Test PASSED: All expected frequencies were detected!" << std::endl;
        return 0;
    } else {
        std::cerr << "✗ Test FAILED: Not all expected frequencies were detected." << std::endl;
        return 1;
    }
}