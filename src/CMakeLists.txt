# src/CMakeLists.txt

add_subdirectory(input)
add_subdirectory(compute)
add_subdirectory(chord-detection)

find_library(FFTW_LIB fftw3f REQUIRED)
include_directories(/usr/include)

find_package(ALSA REQUIRED)

add_executable(main main.cpp)
target_include_directories(main PRIVATE chord-detection compute input)
target_link_libraries(main audio_input asound ${FFTW_LIB} chord_detection -lm)