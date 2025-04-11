#include "audio_input.hpp"

// use the newer ALSA API
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <array>
#include <functional>
#include <iostream>
#include <cstdint>

/* Init 
 *
 * Configure the ALSA input device 
 */
void AudioInput::init()
{
    int rc; // function return code
    unsigned int val;
    int dir;

    // Open default PCM device for audio capture
    // defualt device is configured using alsaconfig
    rc = snd_pcm_open(&handle, "default",
    SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
        "unable to open pcm device: %s\n",
        snd_strerror(rc));
        exit(1);
    }

    // allocate a hardware parameters object
    // fill it in with default values
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    // set the desired hardware parameters
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); // interleaved mode
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE); // signed 16-bit little-endian format
    snd_pcm_hw_params_set_channels(handle, params, 2); // 1 channel (mono guitar input)

    // 8 kHz sampling rate
    val = 8000;
    snd_pcm_hw_params_set_rate_near(handle, params,
                            &val, &dir);

    // set period size to 32 frames
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle,
                        params, &frames, &dir);

    // write the parameters to the driver
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr,
        "unable to set hw parameters: %s\n",
        snd_strerror(rc));
        exit(1);
    }

    // configure buffer to hold 1 period of frames
    snd_pcm_hw_params_get_period_size(params,
        &frames, &dir);
    size = frames * 4; // 2 bytes/sample, 2 channels
    buffer = (char *) malloc(size);

    std::cout << frames << " Frames" << std::endl;

    if (size != (sample_array_size*2))
    {
       fprintf(stderr, "buffer too small. Buffer size: %d", size);
       exit(1);
    }

}

void AudioInput::start_loop()
{
    // start the input reading loop
    while (!done){
        input_loop();
    }
}


void AudioInput::stop_loop()
{
    done = true;
}

/* Close
 *
 * Close down the ALSA connection and free memory
 */
void AudioInput::close()
{
    snd_pcm_drop(handle);
    snd_pcm_close(handle);
    free(buffer);
}

void AudioInput::input_loop()
{
    int rc;

    // read samples in from buffer
    // the snd_pcm_readi() function is blocking and reads in interleaved data
    rc = snd_pcm_readi(handle, buffer, frames);
    if (rc == -EPIPE) {
        /* EPIPE means overrun */
        fprintf(stderr, "overrun occurred\n");
        snd_pcm_prepare(handle);
    } else if (rc < 0) {
        fprintf(stderr,
                "error from read: %s\n",
                snd_strerror(rc));
    } else if (rc != (int)frames) {
        fprintf(stderr, "short read, read %d frames\n", rc);
    }

    // copy the buffer into C++ array object
    std::copy(buffer, buffer+size, sample_array.begin());

    // call the callback function with the new data
    callback_function(sample_array);
}

/* Register callback
 *
 * Register the function that will be called when a new audio sample buffer is ready
 */
void AudioInput::register_callback(std::function<void(std::array<uint16_t, sample_array_size>&)> new_callback)
{
    callback_function = new_callback;
}
