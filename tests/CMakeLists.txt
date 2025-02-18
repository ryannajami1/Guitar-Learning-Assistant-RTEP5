# tests/CMakeLists.txt
target_include_directories(mult5 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(test_mult5 test_mult5.cpp)
target_link_libraries(test_mult5 mult5)

add_test(TestMult5 test_mult5)

add_executable(test_simple test_simple.cpp)
add_test(TestSimple test_simple)
