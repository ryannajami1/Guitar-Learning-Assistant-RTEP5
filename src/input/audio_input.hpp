#ifndef AUDIO_INPUT_HPP
#define AUDIO_INPUT_HPP

#include "biquad_bandstop.hpp"
#include <alsa/asoundlib.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>

constexpr size_t sample_array_size = 128;

class AudioInput{
public:
    void init();
    void start_loop();
    void stop_loop();
    void close();
    void record_period(std::array<int16_t, sample_array_size>& sample_array);

    void register_callback(std::function<void(std::vector<int16_t>&)> new_callback);

private:
    // callback function to be called when data is received
    std::function<void(std::vector<int16_t>&)> callback_function;

    snd_pcm_t* handle; // pointer to audio stream handle
    snd_pcm_hw_params_t* params; // pointer to audio stream parameters
    snd_pcm_uframes_t frames; // number of frames per period

    snd_pcm_uframes_t size; // size of each frame in bytes
    int16_t* buffer; // buffer to store single frame

    std::array<int16_t, sample_array_size> sample_array;
    std::vector<int16_t> recording_buffer;

    BiquadBandstop bandstop_filter; // notch filter to remove 50 Hz

    bool done = false;
    bool recording = false;
    int periods_recorded = 0;
    int num_periods = 32; // number of periods to record in a buffer

    const int saturation_threshold = 32767;
    const int chord_threshold = 800;
};

#endif
