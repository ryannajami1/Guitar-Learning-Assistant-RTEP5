/*
 Test program for the audio input code
 Prints each audio sample to stdout
*/
#include "audio_input.hpp"

#include <cstdint>
#include <iostream>

AudioInput audio_in;

void Callback(std::vector<int16_t> &sample_buffer) {
  std::cerr << "Chord detected" << std::endl;

  for (const auto &val : sample_buffer) {
    std::cout << val << std::endl;
  }
}

auto main() -> int {
  std::cerr << "Chord test" << std::endl;

  audio_in.register_callback(Callback);
  audio_in.init();
  audio_in.start_loop();
  audio_in.close();
}
