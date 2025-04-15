#include "../src/compute/guitarfft.cpp"
#include <cmath>
#include <vector>
#include <array>

// Function to generate a sine wave at a specific frequency
void generate_sine_wave(std::array<int16_t, FRAMES>& buffer, float frequency, float amplitude, float sample_rate) {
    for (size_t i = 0; i < buffer.size(); i++) {
        float time = static_cast<float>(i) / sample_rate;
        float value = amplitude * sin(2.0f * M_PI * frequency * time);
        buffer[i] = static_cast<int16_t>(value);
    }
}

// Function to generate a guitar chord (multiple frequencies)
void generate_guitar_chord(std::array<int16_t, FRAMES>& buffer, const std::vector<float>& frequencies, float amplitude, float sample_rate) {
    std::fill(buffer.begin(), buffer.end(), 0);
    
    // Temporary buffer for each frequency
    std::array<int16_t, FRAMES> temp_buffer;
    
    // Add each frequency component
    for (const auto& freq : frequencies) {
        generate_sine_wave(temp_buffer, freq, amplitude / frequencies.size(), sample_rate);
        
        // Sum into the main buffer
        for (size_t i = 0; i < buffer.size(); i++) {
            // Scale down to avoid overflow
            buffer[i] += temp_buffer[i];
        }
    }
}

// Test function that checks if the detected frequencies match the expected ones
bool validate_frequencies(const std::vector<std::pair<float, float>>& detected_peaks, 
                         const std::vector<float>& expected_frequencies,
                         float tolerance = 10.0f) {
    // Check that we detect at least one peak
    if (detected_peaks.empty()) {
        std::cerr << "No frequency peaks detected!" << std::endl;
        return false;
    }
    
    // For each expected frequency, check if it was detected
    for (const auto& expected_freq : expected_frequencies) {
        bool found = false;
        
        // Look for a matching peak within tolerance
        for (const auto& peak : detected_peaks) {
            if (std::abs(peak.first - expected_freq) <= tolerance) {
                found = true;
                std::cout << "Expected frequency " << expected_freq << " Hz matched with detected " 
                         << peak.first << " Hz (magnitude: " << peak.second << ")" << std::endl;
                break;
            }
        }
        
        if (!found) {
            std::cerr << "Failed to detect expected frequency: " << expected_freq << " Hz" << std::endl;
            return false;
        }
    }
    
    return true;
}

auto main() -> int {
    // Sample rate used by the FFT processor
    const float sample_rate = 2000.0f;
    
    // Initialize FFT processor
    GuitarFFTProcessor processor(FRAMES, sample_rate, FRAMES);
    if (!processor.initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    // Test with a simple E major chord (E, G#, B)
    std::vector<float> e_chord_frequencies = {
        82.41f,  // E2
        164.81f, // E3 (first harmonic)
        207.65f, // G#3
        246.94f  // B3
    };
    
    std::cout << "Testing E major chord detection..." << std::endl;
    
    // Create buffer for the simulated chord
    std::array<int16_t, FRAMES> buffer;
    
    // Generate enough frames to trigger FFT processing
    bool test_passed = true;
    for (unsigned int i = 0; i < processor.get_frames_needed(); i++) {
        // Generate a frame with the E chord frequencies
        generate_guitar_chord(buffer, e_chord_frequencies, 16000.0f, sample_rate);
        
        // Add the frame to the processor
        processor.add_frame(buffer);
    }
    
    // Process the last batch if needed
    processor.process_frames();
    
    // Get the detected peaks
    auto detected_peaks = processor.get_frequency_peaks();
    
    // Validate the results
    if (!validate_frequencies(detected_peaks, e_chord_frequencies)) {
        test_passed = false;
    }
    
    // Clean up
    processor.cleanup();
    
    return test_passed ? 0 : 1;
}