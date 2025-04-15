/*
 Test program for the audio input code
 Prints each audio sample to stdout
*/
#include "audio_input.hpp"

#include <cstdint>
#include <iostream>

AudioInput in;

void callback(std::vector<int16_t>& sample_buffer)
{
    std::cerr << "Chord detected" << std::endl;

    for (const auto& val : sample_buffer) {
        std::cout << val << std::endl;
    }
}

int main() {    
    std::cerr << "Chord test" <<std::endl;

    in.register_callback(callback);
    in.init();
    in.start_loop();
    in.close();
}
