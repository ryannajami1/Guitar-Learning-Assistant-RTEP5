#ifndef AUDIO_INPUT_HPP
#define AUDIO_INPUT_HPP

#include <alsa/asoundlib.h>
#include <array>
#include <functional>
#include <cstdint>

constexpr size_t sample_array_size = 128;

class AudioInput{
public:
    void init();
    void start_loop();
    void stop_loop();
    void close();

    void register_callback(std::function<void(std::array<int16_t, sample_array_size>&)> new_callback);

private:
    // callback function to be called when data is received
    std::function<void(std::array<int16_t, sample_array_size>&)> callback_function;

    snd_pcm_t* handle; // pointer to audio stream handle
    snd_pcm_hw_params_t* params; // pointer to audio stream parameters
    snd_pcm_uframes_t frames; // number of frames per period

    int size; // size of each frame in bytes
    int16_t* buffer; // buffer to store single frame

    std::array<int16_t, sample_array_size> sample_array;

    bool done = false;
};

#endif
