# src/compute/CMakeLists.txt

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(FFTW3 REQUIRED fftw3)

# include_directories(${FFTW3_INCLUDE_DIRS})
# link_directories(${FFTW3_LIBRARY_DIRS})

find_library(FFTW_LIB fftw3f REQUIRED)
include_directories(/usr/include)

find_package(ALSA REQUIRED)

add_executable(guitarfft guitarfft.cpp)

target_include_directories(guitarfft PRIVATE ../input ../chord-detection)
target_link_libraries(guitarfft audio_input asound ${FFTW_LIB} chord_detection -lm)

add_library(guitarfft guitarfft.cpp)

add_executable(mainfft mainfft.cpp)

target_link_libraries(mainfft guitarfft audio_input asound pthread ${FFTW_LIB} -lm)

