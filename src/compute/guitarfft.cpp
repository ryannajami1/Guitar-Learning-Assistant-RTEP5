#include "fftw3.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <memory>
#include <thread>
#include <array>
#include "audio_input.hpp"

#define FRAMES 128

/**
 * GuitarFFTProcessor
 * 
 * Processes guitar audio signals using FFT to detect dominant frequencies.
 * Modified to work with small frame sizes (No. of samples) by collecting multiple frames.
 */
class GuitarFFTProcessor {
private:
    // FFT parameters
    unsigned int fft_size;
    unsigned int sample_rate;
    unsigned int samples_per_frame;
    unsigned int frames_to_collect;
    
    // FFTW data structures
    fftwf_plan fft_plan;
    float* input_buffer;
    fftwf_complex* output_buffer;
    
    // Circular buffer to store multiple frames
    std::vector<short> frame_buffer;
    unsigned int buffer_position;
    unsigned int frames_collected;
    
    // Windowing function (Hann window)
    std::vector<float> window;
    
    // Output frequency peak data
    struct FrequencyPeak {
        float frequency;
        float magnitude;
        
        // For sorting peaks by magnitude
        bool operator<(const FrequencyPeak& other) const {
            return magnitude > other.magnitude; // Sort in descending order
        }
    };
    
    std::vector<FrequencyPeak> frequency_peaks;
    
    // Create Hann window for reducing spectral leakage
    void create_window() {
        window.resize(fft_size);
        for (unsigned int i = 0; i < fft_size; i++) {
            window[i] = 0.5f * (1.0f - cos(2.0f * M_PI * i / (fft_size - 1)));
        }
    }
    
    // Find peaks in the frequency spectrum
    void find_frequency_peaks(float threshold_percent = 5.0f, std::size_t max_peaks = 10) {
        frequency_peaks.clear();
        
        // Calculate maximum magnitude for threshold calculation
        float max_magnitude = 0.0f;
        for (unsigned int i = 1; i < fft_size / 2; i++) {
            float real = output_buffer[i][0];
            float imag = output_buffer[i][1];
            float magnitude = sqrt(real * real + imag * imag);
            if (magnitude > max_magnitude) {
                max_magnitude = magnitude;
            }
        }
        
        // Calculate threshold value
        float threshold = max_magnitude * threshold_percent / 100.0f;
        
        // Find local maxima that exceed the threshold
        for (unsigned int i = 2; i < fft_size / 2 - 2; i++) {
            float real = output_buffer[i][0];
            float imag = output_buffer[i][1];
            float magnitude = sqrt(real * real + imag * imag);
            
            // Check if this bin is a local maximum and exceeds threshold
            if (magnitude > threshold) {
                float prev_mag1 = sqrt(output_buffer[i-1][0] * output_buffer[i-1][0] + 
                                     output_buffer[i-1][1] * output_buffer[i-1][1]);
                float prev_mag2 = sqrt(output_buffer[i-2][0] * output_buffer[i-2][0] + 
                                     output_buffer[i-2][1] * output_buffer[i-2][1]);
                float next_mag1 = sqrt(output_buffer[i+1][0] * output_buffer[i+1][0] + 
                                     output_buffer[i+1][1] * output_buffer[i+1][1]);
                float next_mag2 = sqrt(output_buffer[i+2][0] * output_buffer[i+2][0] + 
                                     output_buffer[i+2][1] * output_buffer[i+2][1]);
                
                if (magnitude > prev_mag1 && magnitude > prev_mag2 && 
                    magnitude > next_mag1 && magnitude > next_mag2) {
                    
                    // Calculate the frequency
                    float frequency = static_cast<float>(i) * sample_rate / fft_size;
                    
                    // Skip very low frequencies (DC and near-DC components)
                    if (frequency < 50.0f)
                        continue;
                    
                    // Add to peaks
                    frequency_peaks.push_back({frequency, magnitude});
                }
            }
        }
        
        // Sort peaks by magnitude (descending)
        std::sort(frequency_peaks.begin(), frequency_peaks.end());
        
        // Limit to top max_peaks (or fewer if there aren't that many)
        if (frequency_peaks.size() > max_peaks) {
            frequency_peaks.resize(max_peaks);
        }
    }

public:
    GuitarFFTProcessor(unsigned int frame_size = FRAMES, 
                       unsigned int rate = 2000,
                       unsigned int frames_to_process = FRAMES) 
    : fft_size(frame_size * frames_to_process),
      sample_rate(rate),
      samples_per_frame(frame_size),
      frames_to_collect(frames_to_process),
      input_buffer(nullptr),
      output_buffer(nullptr), 
      frame_buffer(frame_size * frames_to_process),
      buffer_position(0),
      frames_collected(0) {
    }
    
    ~GuitarFFTProcessor() {
        cleanup();
    }
    
    // Initialize FFTW
    bool initialize() {
        // Allocate FFTW buffers
        input_buffer = (float*)fftwf_malloc(sizeof(float) * fft_size);
        output_buffer = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (fft_size / 2 + 1));
        
        if (!input_buffer || !output_buffer) {
            std::cerr << "Failed to allocate FFTW buffers" << std::endl;
            return false;
        }
        
        // Create FFT plan
        fft_plan = fftwf_plan_dft_r2c_1d(fft_size, input_buffer, output_buffer, FFTW_MEASURE);
        
        if (!fft_plan) {
            std::cerr << "Failed to create FFTW plan" << std::endl;
            return false;
        }
        
        // Initialize window function
        create_window();
        
        // Clear the frame buffer
        std::fill(frame_buffer.begin(), frame_buffer.end(), 0);
        
        return true;
    }
    
    // Add a frame to the buffer and process if we have enough frames
    void add_frame(std::array<int16_t, FRAMES>& frame) {
        // Copy the frame into buffer
        std::memcpy(frame_buffer.data() + buffer_position, frame.begin(), samples_per_frame * sizeof(int16_t));
        
        // Update position and frame count
        buffer_position += samples_per_frame;
        if (buffer_position >= frame_buffer.size()) {
            buffer_position = 0; // Wrap around if needed
        }
        
        frames_collected++;
        
        // If we have enough frames, process them
        if (frames_collected >= frames_to_collect) {
            // Process the collected frames
            process_frames();

            // Finlay
            
            // Reset frame counter but keep the buffer position
            // (this creates a sliding window effect)
            frames_collected = frames_to_collect / 2; // 50% overlap
            
            // return true; // Indicate that processing occurred
        }
        
        // return false; // Indicate that we're still collecting frames
    }
    
    // Process the collected frames
    void process_frames() {
        // Copy data to FFTW input buffer with conversion to float and windowing
        for (unsigned int i = 0; i < fft_size; i++) {
            // Apply circular buffer logic to get the right sample
            unsigned int buffer_index = (buffer_position + i) % frame_buffer.size();
            input_buffer[i] = (frame_buffer[buffer_index] / 32768.0f) * window[i];
        }
        
        // Perform FFT
        fftwf_execute(fft_plan);
        
        // Find frequency peaks
        find_frequency_peaks();
        print_frequency_peaks();
    }
    
    // Get frequency peaks for chord detection
    std::vector<std::pair<float, float>> get_frequency_peaks() const {
        std::vector<std::pair<float, float>> result;
        result.reserve(frequency_peaks.size());
        
        for (const auto& peak : frequency_peaks) {
            result.push_back({peak.frequency, peak.magnitude});
        }
        
        return result;
    }
    
    // Write frequency peaks to a file for the chord detection module
    void write_frequency_data_to_file(const std::string& filename) {
        std::ofstream outfile(filename, std::ios::binary);
        if (!outfile.is_open()) {
            std::cerr << "Failed to open output file: " << filename << std::endl;
            return;
        }
        
        // Write the number of peaks
        size_t num_peaks = frequency_peaks.size();
        outfile.write(reinterpret_cast<const char*>(&num_peaks), sizeof(num_peaks));
        
        // Write each peak
        for (const auto& peak : frequency_peaks) {
            outfile.write(reinterpret_cast<const char*>(&peak.frequency), sizeof(peak.frequency));
            outfile.write(reinterpret_cast<const char*>(&peak.magnitude), sizeof(peak.magnitude));
        }
        
        outfile.close();
    }
    
    // Print frequency peaks to console
    void print_frequency_peaks() {
        std::cout << "--- Detected Frequency Peaks ---" << std::endl;
        for (const auto& peak : frequency_peaks) {
            std::cout << "Frequency: " << std::fixed << std::setprecision(2) << peak.frequency 
                     << " Hz, Magnitude: " << peak.magnitude << std::endl;
        }
        std::cout << "--------------------------------" << std::endl;
    }
    
    // Get the total frames collected so far
    unsigned int get_frames_collected() const {
        return frames_collected;
    }
    
    // Get the number of frames needed for processing
    unsigned int get_frames_needed() const {
        return frames_to_collect;
    }
    
    // Clean up resources
    void cleanup() {
        if (fft_plan) {
            fftwf_destroy_plan(fft_plan);
            fft_plan = nullptr;
        }
        
        if (input_buffer) {
            fftwf_free(input_buffer);
            input_buffer = nullptr;
        }
        
        if (output_buffer) {
            fftwf_free(output_buffer);
            output_buffer = nullptr;
        }
    }
};


int main() {
    // Create processor for Sampled frames at 2kHz
    // Will collect frames (total 1024 samples) before processing
    GuitarFFTProcessor processor(FRAMES, 2000, FRAMES);
    AudioInput in;
    
    // Initialize the processor
    if (!processor.initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    in.register_callback([&processor](auto frame) {
        processor.add_frame(frame);
    });

    in.init();

    std::thread t([&in]() {in.start_loop();});
    
    t.join();

    return 0;
}
