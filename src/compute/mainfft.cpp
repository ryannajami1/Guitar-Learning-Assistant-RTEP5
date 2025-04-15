#include "fftw3.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <memory>
#include <thread>
#include <array>
#include "guitarfft.cpp"
#include "audio_input.hpp"

int main() {
    // Create processor for Sampled frames at 2kHz
    // Will collect frames (total 1024 samples) before processing
    GuitarFFTProcessor processor(FRAMES, 2000, FRAMES);
    AudioInput in;
    
    // Initialize the processor
    if (!processor.initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    in.register_callback([&processor](auto frame) {
        processor.add_frame(frame);
    });

    in.init();

    std::thread t([&in]() {in.start_loop();});
    
    t.join();

    return 0;
}