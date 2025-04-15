#ifndef GUITARFFT_PROCESSOR_HPP
#define GUITARFFT_PROCESSOR_HPP

#include <array>
#include <vector>
#include <string>

// Forward declaration of FFTW types
struct fftwf_plan_s;
typedef fftwf_plan_s* fftwf_plan;
typedef float fftwf_complex;

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
    float* input_buffer_;
    fftwf_complex* output_buffer_;

    // Circular buffer to store multiple frames
    std::vector<short> frame_buffer_;
    unsigned int buffer_position_;
    unsigned int frames_collected_;

    // Windowing function (Hann window)
    std::vector<float> window_;

    // Output frequency peak data
    struct FrequencyPeak {
        float frequency;
        float magnitude;

        // For sorting peaks by magnitude
        bool operator<(const FrequencyPeak& other) const;
    };

    std::vector<FrequencyPeak> frequency_peaks_;

    // Create Hann window for reducing spectral leakage
    void CreateWindow();

    // Find peaks in the frequency spectrum
    void FindFrequencyPeaks(float threshold_percent = 5.0F, std::size_t max_peaks = 10);

    // Process the collected frames
    void ProcessFrames();

    // Clean up resources
    void Cleanup();

public:
    // Constructor
    explicit GuitarFFTProcessor(unsigned int frame_size = FRAMES, unsigned int rate = 2000, unsigned int frames_to_process = FRAMES);

    // Destructor
    ~GuitarFFTProcessor();

    // Initialize FFTW
    bool Initialize();

    // Add a frame to the buffer and process if we have enough frames
    void AddFrame(std::array<int16_t, FRAMES>& frame);

    // Get frequency peaks for chord detection
    [[nodiscard]] std::vector<std::pair<float, float>> GetFrequencyPeaks() const;

    // Write frequency peaks to a file for the chord detection module
    void WriteFrequencyDataToFile(const std::string& filename);

    // Print frequency peaks to console
    void PrintFrequencyPeaks();

    // Get the total frames collected so far
    [[nodiscard]] unsigned int GetFramesCollected() const;

    // Get the number of frames needed for processing
    [[nodiscard]] unsigned int GetFramesNeeded() const;
};

#endif // GUITARFFT_PROCESSOR_HPP