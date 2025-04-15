#include "../input/audio_input.hpp"
#include "fftw3.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

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
  unsigned int fft_size_;
  unsigned int sample_rate_;
  unsigned int samples_per_frame_;
  unsigned int frames_to_collect_;

  // FFTW data structures
  fftwf_plan fft_plan_;
  float *input_buffer_{nullptr};
  fftwf_complex *output_buffer_{nullptr};

  // Circular buffer to store multiple frames
  std::vector<short> frame_buffer_;
  unsigned int buffer_position_{0};
  unsigned int frames_collected_{0};

  // Windowing function (Hann window)
  std::vector<float> window_;

  // Output frequency peak data
  struct FrequencyPeak {
    float frequency;
    float magnitude;

    // For sorting peaks by magnitude
    auto operator<(const FrequencyPeak &other) const -> bool {
      return magnitude > other.magnitude; // Sort in descending order
    }
    };

    std::vector<FrequencyPeak> frequency_peaks_;

    // Create Hann window for reducing spectral leakage
    void CreateWindow() {
      window_.resize(fft_size_);
      for (unsigned int i = 0; i < fft_size_; i++) {
        window_[i] = 0.5F * (1.0F - cos(2.0F * M_PI * i / (fft_size_ - 1)));
      }
    }

    // Find peaks in the frequency spectrum
    void FindFrequencyPeaks(float threshold_percent = 5.0F,
                            std::size_t max_peaks = 10) {
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

public:
  explicit GuitarFFTProcessor(unsigned int frame_size = FRAMES,
                              unsigned int rate = 2000,
                              unsigned int frames_to_process = FRAMES)
      : fft_size_(frame_size * frames_to_process), sample_rate_(rate),
        samples_per_frame_(frame_size), frames_to_collect_(frames_to_process),

        frame_buffer_(frame_size * frames_to_process) {}

  ~GuitarFFTProcessor() { Cleanup(); }

  // Initialize FFTW
  auto Initialize() -> bool {
    // Allocate FFTW buffers
    input_buffer_ =
        static_cast<float *>(fftwf_malloc(sizeof(float) * fft_size_));
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

    // Add a frame to the buffer and process if we have enough frames
  void AddFrame(std::array<int16_t, FRAMES> &frame) {
    // Copy the frame into buffer
    std::memcpy(frame_buffer_.data() + buffer_position_, frame.begin(),
                samples_per_frame_ * sizeof(int16_t));

    // Update position and frame count
    buffer_position_ += samples_per_frame_;
    if (buffer_position_ >= frame_buffer_.size()) {
      buffer_position_ = 0; // Wrap around if needed
    }

    frames_collected_++;

    // If we have enough frames, process them
    if (frames_collected_ >= frames_to_collect_) {
      // Process the collected frames
      ProcessFrames();

      // Finlay

      // Reset frame counter but keep the buffer position
      // (this creates a sliding window effect)
      frames_collected_ = frames_to_collect_ / 2; // 50% overlap

      // return true; // Indicate that processing occurred
    }

    // return false; // Indicate that we're still collecting frames
  }

    // Process the collected frames
  void ProcessFrames() {
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
  }

    // Get frequency peaks for chord detection
  [[nodiscard]] auto GetFrequencyPeaks() const
      -> std::vector<std::pair<float, float>> {
    std::vector<std::pair<float, float>> result;
    result.reserve(frequency_peaks_.size());

    for (const auto &peak : frequency_peaks_) {
      result.emplace_back(peak.frequency, peak.magnitude);
    }

    return result;
  }

    // Write frequency peaks to a file for the chord detection module
  void WriteFrequencyDataToFile(const std::string &filename) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
      std::cerr << "Failed to open output file: " << filename << std::endl;
      return;
    }

    // Write the number of peaks
    size_t num_peaks = frequency_peaks_.size();
    outfile.write(reinterpret_cast<const char *>(&num_peaks),
                  sizeof(num_peaks));

    // Write each peak
    for (const auto &peak : frequency_peaks_) {
      outfile.write(reinterpret_cast<const char *>(&peak.frequency),
                    sizeof(peak.frequency));
      outfile.write(reinterpret_cast<const char *>(&peak.magnitude),
                    sizeof(peak.magnitude));
    }

    outfile.close();
  }

    // Print frequency peaks to console
  void PrintFrequencyPeaks() {
    std::cout << "--- Detected Frequency Peaks ---" << std::endl;
    for (const auto &peak : frequency_peaks_) {
      std::cout << "Frequency: " << std::fixed << std::setprecision(2)
                << peak.frequency << " Hz, Magnitude: " << peak.magnitude
                << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
  }

    // Get the total frames collected so far
  [[nodiscard]] auto GetFramesCollected() const -> unsigned int {
    return frames_collected_;
  }

    // Get the number of frames needed for processing
  [[nodiscard]] auto GetFramesNeeded() const -> unsigned int {
    return frames_to_collect_;
  }

    // Clean up resources
  void Cleanup() {
    if (fft_plan_ != nullptr) {
      fftwf_destroy_plan(fft_plan_);
      fft_plan_ = nullptr;
    }

    if (input_buffer_ != nullptr) {
      fftwf_free(input_buffer_);
      input_buffer_ = nullptr;
    }

    if (output_buffer_ != nullptr) {
      fftwf_free(output_buffer_);
      output_buffer_ = nullptr;
    }
  }
};



