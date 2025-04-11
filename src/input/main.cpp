/*
 Test program for the audio input code
 Prints each audio sample to stdout
*/
#include "audio_input.hpp"

#include <cstdint>
#include <iostream>

AudioInput in;

int frame_counter = 0;
int max_frames = 16000; // approx 2s worth of frames

void callback(std::array<uint16_t, sample_array_size>& sample_array)
{
    frame_counter += 32;

    for (unsigned int i=0; i<sample_array_size; i++)
    {
        std::cout << sample_array[i];    
    }
    std::cout << std::endl;

    if (frame_counter >= max_frames)
    {
        in.stop_loop();
    }
}

int main() {    
    in.register_callback(callback);
    in.init();
    in.start_loop();
    in.close();
}