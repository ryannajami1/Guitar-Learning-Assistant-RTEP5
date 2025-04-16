#include <iostream>
#include <string>
#include "guitarfft.hpp"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include "audio_input.hpp"
#include "ChordDetection.hpp"


int main() {
    // Create processor for Sampled frames at 2kHz
    // Will collect frames (total 1024 samples) before processing
    GuitarFFTProcessor processor(FRAMES, 2000, FRAMES);
    AudioInput in;
    
    // Initialize the processor
    if (!processor.Initialize()) {
        std::cerr << "Failed to initialize FFT processor" << std::endl;
        return 1;
    }
    
    in.register_callback([&processor](auto frame) {
        processor.ProcessFrames(frame);
    });

    in.init();

    std::thread t([&in]() {in.start_loop();});
    
    t.join();

    return 0;
}