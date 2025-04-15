/* AUDIO INPUT
 * Craig Cochrane, 2025
 *
 * Get samples from ALSA PCM device and pass array to a callback function to calculate the FFT
 * ALSA is configured to send 128 16-bit mono samples per period at a sameple rate of 8kHz
 *
 * TO DO:
 * - Add filtering for input data to allow for downsampling
 * - Could check for a chord being played () and only pass data to the FFT when a chord has been played
 * - Could add check for the input saturating and output an error message (or reduce gain directly in the program?)
 */

#include "audio_input.hpp"

// use the newer ALSA API
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <array>
#include <functional>
#include <iostream>
#include <cstdint>
#include <thread>

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

    // get the minimum number of channels, sampling rate and period size
    unsigned int min_val;
    snd_pcm_hw_params_get_channels_min(params, &min_val);
    std::cerr << "Minimum channels: " << min_val << std::endl;
    snd_pcm_hw_params_get_rate_min(params, &min_val, nullptr);
    std::cerr << "Minimum sample rate: " << min_val << "Hz" << std::endl;
    snd_pcm_uframes_t min_per;
    snd_pcm_hw_params_get_period_size_min(params, &min_per, nullptr);
    std::cerr << "Minimum period: " << min_per << std::endl;

    // set the desired hardware parameters
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); // interleaved mode
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE); // signed 16-bit little-endian format
    snd_pcm_hw_params_set_channels(handle, params, 1); // 1 channel (mono guitar input)

    // 8 kHz sampling rate
    val = 7999; // for some reason ALSA adds 1
    snd_pcm_hw_params_set_rate_near(handle, params,
                            &val, &dir);

    // set period size to 128 frames
    frames = 127; // for some reason ALSA adds 1
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

    // output the parameters that have actually been set
    snd_pcm_hw_params_get_channels(params, &min_val);
    std::cerr << "Number of channels: " << min_val << std::endl;
    snd_pcm_hw_params_get_rate(params, &min_val, nullptr);
    std::cerr << "Sample rate: " << min_val << "Hz" << std::endl;
    snd_pcm_hw_params_get_period_size(params,&frames, &dir);
    std::cerr << "Number of frames per period: " << frames << std::endl;

    // configure buffer to hold 1 period of frames
    size = frames; // only single channel used
    buffer = (int16_t *) malloc(size);

    if (size != sample_array_size)
    {
       fprintf(stderr, "buffer incorrect size. Buffer size: %d, array: %zu\n", size, sample_array_size);
       exit(1);
    }

}


/* Start Loop
 *
 * Start an infinite loop where the audio device is read from and the callback function called
 * Loop ends when the done variable is set to false by the end loop function
 */
void AudioInput::start_loop()
{
    int rc;

    // start the input reading loop
    while (!done){
    	// read samples in from buffer
    	// the snd_pcm_readi() function is blocking and reads in interleaved data
	    // the sound device is configured for mono, so only 1 channel is actually read in
    	rc = snd_pcm_readi(handle, buffer, frames);
    	if (rc == -EPIPE) {
        	// EPIPE means overrun in the ALSA buffer
	        fprintf(stderr, "overrun occurred\n");
        	snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
        }

        // copy the buffer into C++ array object
        std::copy(buffer, buffer+size, sample_array.begin());

        // call the callback function with the new data in a new thread
        std::thread thr(callback_function, sample_array);

        // allow the thread to run with the signal processing
        // loop will return to the blocking ALSA read
        thr.detach();
    }
}

/* Stop Loop
 *
 * Set the done variable so that the loop is interrupted
 */
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
    snd_pcm_drop(handle); // discards remaining samples in the ALSA buffer
    snd_pcm_close(handle); 
    free(buffer); 
}

/* Register callback
 *
 * Register the function that will be called when a new audio sample buffer is ready
 */
void AudioInput::register_callback(std::function<void(std::array<int16_t, sample_array_size>&)> new_callback)
{
    callback_function = new_callback;
}
