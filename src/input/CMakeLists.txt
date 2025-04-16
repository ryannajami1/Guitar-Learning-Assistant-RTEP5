# src/input/CMakeLists.txt

add_library(audioInput audio_input.cpp biquad_bandstop.cpp)

target_include_directories(audio_input PUBLIC .)

find_package(ALSA REQUIRED)

add_executable(audio_test main.cpp)
target_link_libraries(audio_test audio_input asound)