# src/output/CMakeLists.txt

add_library(demobaker DemoBaker.cpp)

target_include_directories(demobaker PUBLIC . ../input)

target_link_libraries(demobaker demodoughproducer)
