# src/compute/CMakeLists.txt

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3)


include_directories(${FFTW3_INCLUDE_DIRS})
link_directories(${FFTW3_LIBRARY_DIRS})

add_executable(guitarfft guitarfft.cpp)
target_link_libraries(guitarfft audio_input ${FFTW3_LIBRARIES})
target_include_directories(guitarfft PRIVATE ../input)

