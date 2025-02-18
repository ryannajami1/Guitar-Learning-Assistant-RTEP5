# src/compute/CMakeLists.txt

add_executable(demoorganiser DemoOrganiser.cpp)
target_include_directories(demoorganiser PUBLIC . ../input ../output)

target_link_libraries(demoorganiser demodoughproducer)
target_link_libraries(demoorganiser demobaker)
