# src/CMakeLists.txt

add_library(mult5 mult5.cpp)

target_include_directories(mult5 PUBLIC .)

add_executable(helloworld helloworld.cpp)
target_link_libraries(helloworld mult5)


add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(compute)
add_subdirectory(chord-detection)