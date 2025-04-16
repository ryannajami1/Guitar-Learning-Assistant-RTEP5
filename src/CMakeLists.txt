# src/CMakeLists.txt
set(libwebsockets_lib "/usr/lib/aarch64-linux-gnu/libwebsockets.so.17")

add_subdirectory(input)
add_subdirectory(compute)
add_subdirectory(chord-detection)
add_subdirectory(websocket)

find_library(FFTW_LIB fftw3f REQUIRED)
include_directories(/usr/include)

find_package(ALSA REQUIRED)

add_executable(main main.cpp)
target_include_directories(main PRIVATE chord-detection compute input websocket)
target_link_libraries(main guitarfft audio_input asound ${FFTW_LIB} chord_detection websocket ${libwebsockets_lib} -lm)
