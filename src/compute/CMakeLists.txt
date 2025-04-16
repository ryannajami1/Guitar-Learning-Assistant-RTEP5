# src/compute/CMakeLists.txt

find_library(FFTW_LIB fftw3f REQUIRED)
include_directories(/usr/include)

find_package(ALSA REQUIRED)

add_library(guitarfft guitarfft.cpp)

target_include_directories(guitarfft PRIVATE ../input ../chord-detection ../websocket)
target_link_libraries(guitarfft audio_input asound ${FFTW_LIB} chord_detection websocket -lm)
