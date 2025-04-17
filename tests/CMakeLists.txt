# tests/CMakeLists.txt
target_include_directories(chord_detection PRIVATE ${CMAKE_SOURCE_DIR}/src/chord-detection)

# add_executable(test_ChordDetection test_ChordDetection.cpp)
# target_link_libraries(test_ChordDetection chord_detection)

# add_test(TestChordDetection test_ChordDetection)


# find_library(FFTW_LIB fftw3f REQUIRED)
# include_directories(/usr/include)

# find_package(ALSA REQUIRED)

# add_executable(test_fft test_fft.cpp)
# target_include_directories(guitarfft PRIVATE ${CMAKE_SOURCE_DIR}/src/compute)
# target_link_libraries(test_fft guitarfft audio_input asound pthread ${FFTW_LIB} -lm)
# add_test(TestFFT test_fft)

