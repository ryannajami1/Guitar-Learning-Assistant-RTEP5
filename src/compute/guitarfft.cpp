#include "fftw3.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include "guitarfft.hpp"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <array>
#include "audio_input.hpp"
#include "ChordDetection.hpp"
#include <vector>
#include "websocket.hpp"

// Implementation of FrequencyPeak operator<
auto GuitarFFTProcessor::FrequencyPeak::operator<(const FrequencyPeak &other) const -> bool {
    return magnitude > other.magnitude; // Sort in descending order
}

// Create Hann window for reducing spectral leakage
void GuitarFFTProcessor::CreateWindow() {
    window_.resize(fft_size_);
    for (unsigned int i = 0; i < fft_size_; i++) {
        window_[i] = 0.5F * (1.0F - cos(2.0F * M_PI * i / (fft_size_ - 1)));
    }
}

// Find peaks in the frequency spectrum
void GuitarFFTProcessor::FindFrequencyPeaks(float threshold_percent, std::size_t max_peaks) {
    frequency_peaks_.clear();

    // Calculate maximum magnitude for threshold calculation
    float max_magnitude = 0.0F;
    for (unsigned int i = 1; i < fft_size_ / 2; i++) {
        float real = output_buffer_[i][0];
        float imag = output_buffer_[i][1];
        float magnitude = std::sqrt(real * real + imag * imag);
        if (magnitude > max_magnitude) {
            max_magnitude = magnitude;
        }
    }

    // Calculate threshold value
    float threshold = max_magnitude * threshold_percent / 100.0F;

    // Find local maxima that exceed the threshold
    for (unsigned int i = 2; i < fft_size_ / 2 - 2; i++) {
        float real = output_buffer_[i][0];
        float imag = output_buffer_[i][1];
        float magnitude = std::sqrt(real * real + imag * imag);

        // Check if this bin is a local maximum and exceeds threshold
        if (magnitude > threshold) {
            float prev_mag1 =
                std::sqrt(output_buffer_[i - 1][0] * output_buffer_[i - 1][0] +
                        output_buffer_[i - 1][1] * output_buffer_[i - 1][1]);
            float prev_mag2 =
                std::sqrt(output_buffer_[i - 2][0] * output_buffer_[i - 2][0] +
                        output_buffer_[i - 2][1] * output_buffer_[i - 2][1]);
            float next_mag1 =
                std::sqrt(output_buffer_[i + 1][0] * output_buffer_[i + 1][0] +
                        output_buffer_[i + 1][1] * output_buffer_[i + 1][1]);
            float next_mag2 =
                std::sqrt(output_buffer_[i + 2][0] * output_buffer_[i + 2][0] +
                        output_buffer_[i + 2][1] * output_buffer_[i + 2][1]);

            if (magnitude > prev_mag1 && magnitude > prev_mag2 &&
                magnitude > next_mag1 && magnitude > next_mag2) {

                // Calculate the frequency
                float frequency = static_cast<float>(i) * static_cast<float>(sample_rate_) / static_cast<float>(fft_size_);

                // Skip very low frequencies (DC and near-DC components)
                if (frequency < 50.0F) {
                    continue;
                }

                // Add to peaks
                frequency_peaks_.push_back({frequency, magnitude});
            }
        }
    }

    // Sort peaks by magnitude (descending)
    std::sort(frequency_peaks_.begin(), frequency_peaks_.end());

    // Limit to top max_peaks (or fewer if there aren't that many)
    if (frequency_peaks_.size() > max_peaks) {
        frequency_peaks_.resize(max_peaks);
    }
}

// Constructor
GuitarFFTProcessor::GuitarFFTProcessor(unsigned int frame_size, unsigned int rate, unsigned int frames_to_process)
    : fft_size_(frame_size * frames_to_process), 
      sample_rate_(rate),
      samples_per_frame_(frame_size), 
      frames_to_collect_(frames_to_process),
      input_buffer_(nullptr),
      output_buffer_(nullptr),
      frame_buffer_(frame_size * frames_to_process),
      buffer_position_(0),
      frames_collected_(0) {
}

// Destructor
GuitarFFTProcessor::~GuitarFFTProcessor() { 
    Cleanup(); 
}

// Initialize FFTW
auto GuitarFFTProcessor::Initialize() -> bool {
    // Allocate FFTW buffers
    input_buffer_ = static_cast<float *>(fftwf_malloc(sizeof(float) * fft_size_));
    output_buffer_ = static_cast<fftwf_complex *>(
        fftwf_malloc(sizeof(fftwf_complex) * (fft_size_ / 2 + 1)));

    if ((input_buffer_ == nullptr) || (output_buffer_ == nullptr)) {
        std::cerr << "Failed to allocate FFTW buffers" << std::endl;
        return false;
    }

    // Create FFT plan
    fft_plan_ = fftwf_plan_dft_r2c_1d(static_cast<int>(fft_size_), input_buffer_, output_buffer_,
                                    FFTW_MEASURE);

    if (fft_plan_ == nullptr) {
        std::cerr << "Failed to create FFTW plan" << std::endl;
        return false;
    }

    // Initialize window function
    CreateWindow();

    // Clear the frame buffer
    std::fill(frame_buffer_.begin(), frame_buffer_.end(), 0);

    return true;
}

// Process the collected frames
void GuitarFFTProcessor::ProcessFrames(std::vector<int16_t> buf) {
    frame_buffer_ = buf;
    // Copy data to FFTW input buffer with conversion to float and windowing
    for (unsigned int i = 0; i < fft_size_; i++) {
        // Apply circular buffer logic to get the right sample
        unsigned int buffer_index = (buffer_position_ + i) % frame_buffer_.size();
        input_buffer_[i] = (static_cast<float>(frame_buffer_[buffer_index]) / 32768.0F) * window_[i];
    }

    // Perform FFT
    fftwf_execute(fft_plan_);

    // Find frequency peaks
    FindFrequencyPeaks();
    PrintFrequencyPeaks();

    // Find the chord
	ChordDetection cd;

	// Convert fft data to vectors of magnitudes and frequencies
	std::vector<float> fft_mags;
	std::vector<float> fft_freqs;

	for (unsigned int i = 2; i < fft_size_ / 2 - 2; i++) {
        float real = output_buffer_[i][0];
        float imag = output_buffer_[i][1];
        float magnitude = sqrt(real * real + imag * imag);
        fft_mags.push_back(magnitude);

		float frequency = static_cast<float>(i) * sample_rate_ / fft_size_;
		fft_freqs.push_back(frequency);
	}

	// Get the peak frequencies from the FFT data
    std::vector<float> peak_frequencies = cd.GetPeakFrequencies(fft_freqs, fft_mags);

    // Get the chord name from the peak frequencies
	string chord_name = cd.ChordLookup(peak_frequencies);

	std::cout << chord_name << std::endl;

	// Send chord message over websocket
	LWS_SendMessage(chord_name);
}

// Get frequency peaks for chord detection
auto GuitarFFTProcessor::GetFrequencyPeaks() const -> std::vector<std::pair<float, float>> {
    std::vector<std::pair<float, float>> result;
    result.reserve(frequency_peaks_.size());

    for (const auto &peak : frequency_peaks_) {
        result.emplace_back(peak.frequency, peak.magnitude);
    }

    return result;
}

// Write frequency peaks to a file for the chord detection module
void GuitarFFTProcessor::WriteFrequencyDataToFile(const std::string &filename) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file: " << filename << std::endl;
        return;
    }

    // Write the number of peaks
    size_t num_peaks = frequency_peaks_.size();
    outfile.write(reinterpret_cast<const char *>(&num_peaks), sizeof(num_peaks));

    // Write each peak
    for (const auto &peak : frequency_peaks_) {
        outfile.write(reinterpret_cast<const char *>(&peak.frequency), sizeof(peak.frequency));
        outfile.write(reinterpret_cast<const char *>(&peak.magnitude), sizeof(peak.magnitude));
    }

    outfile.close();
}

// Print frequency peaks to console
void GuitarFFTProcessor::PrintFrequencyPeaks() {
    std::cout << "--- Detected Frequency Peaks ---" << std::endl;
    for (const auto &peak : frequency_peaks_) {
        std::cout << "Frequency: " << std::fixed << std::setprecision(2)
                << peak.frequency << " Hz, Magnitude: " << peak.magnitude
                << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}

// Get the total frames collected so far
auto GuitarFFTProcessor::GetFramesCollected() const -> unsigned int {
    return frames_collected_;
}

// Get the number of frames needed for processing
auto GuitarFFTProcessor::GetFramesNeeded() const -> unsigned int {
    return frames_to_collect_;
}

// Clean up resources
void GuitarFFTProcessor::Cleanup() {
    if (fft_plan_ != nullptr) {
        fftwf_destroy_plan(fft_plan_);
        fft_plan_ = nullptr;
    }
};