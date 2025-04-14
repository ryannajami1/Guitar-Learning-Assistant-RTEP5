# src/input/CMakeLists.txt

add_library(audio_input audio_input.cpp)

target_include_directories(audio_input PUBLIC .)

find_package(ALSA REQUIRED)

add_executable(audio_test main.cpp)
target_link_libraries(audio_test audio_input asound)