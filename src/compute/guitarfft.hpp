#pragma once

#include "fftw3.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

enum { FRAMES = 128 };

class GuitarFFTProcessor {
private:
    unsigned int fft_size_;
    unsigned int sample_rate_;
    unsigned int samples_per_frame_;
    unsigned int frames_to_collect_;

    fftwf_plan fft_plan_;
    float *input_buffer_;
    fftwf_complex *output_buffer_;

    std::vector<short> frame_buffer_;
    unsigned int buffer_position_;
    unsigned int frames_collected_;

    std::vector<float> window_;

    struct FrequencyPeak {
        float frequency;
        float magnitude;

        auto operator<(const FrequencyPeak &other) const -> bool;
    };

    std::vector<FrequencyPeak> frequency_peaks_;

    void CreateWindow();
    void FindFrequencyPeaks(float threshold_percent = 5.0F,
                            std::size_t max_peaks = 10);

  public:
    explicit GuitarFFTProcessor(unsigned int frame_size = FRAMES, 
                               unsigned int rate = 2000,
                               unsigned int frames_to_process = FRAMES);
    ~GuitarFFTProcessor();

    auto Initialize() -> bool;
    void AddFrame(std::array<int16_t, FRAMES> &frame);
    void ProcessFrames(std::vector<int16_t> buf);
    std::vector<std::pair<float, float>> GetFrequencyPeaks() const;
    void WriteFrequencyDataToFile(const std::string &filename);
    void PrintFrequencyPeaks();
    unsigned int GetFramesCollected() const;
    unsigned int GetFramesNeeded() const;
    void Cleanup();
};
