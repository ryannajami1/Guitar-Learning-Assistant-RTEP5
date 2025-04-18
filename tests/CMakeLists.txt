# tests/CMakeLists.txt
target_include_directories(chord_detection PRIVATE ${CMAKE_SOURCE_DIR}/src/chord-detection)

add_executable(test_ChordDetection test_ChordDetection.cpp)
target_link_libraries(test_ChordDetection chord_detection)
target_compile_definitions(test_ChordDetection PRIVATE 
    TEST_BUILD
    ROOT_DIR="${CMAKE_SOURCE_DIR}"
)

add_test(TestChordDetection test_ChordDetection)


find_library(FFTW_LIB fftw3f REQUIRED)
include_directories(/usr/include)

find_package(ALSA REQUIRED)

set(libwebsockets_lib "/usr/lib/aarch64-linux-gnu/libwebsockets.so.17")

if(NOT EXISTS libwebsockets_lib)
    set(libwebsockets_lib "/usr/lib/x86_64-linux-gnu/libwebsockets.so.17")
endif()

add_executable(test_fft test_fft.cpp)
target_include_directories(test_fft PRIVATE ${CMAKE_SOURCE_DIR}/src/compute)
target_link_libraries(test_fft guitarfft audio_input asound pthread ${FFTW_LIB} websocket ${libwebsockets_lib} -lm)
add_test(TestFFT test_fft)

