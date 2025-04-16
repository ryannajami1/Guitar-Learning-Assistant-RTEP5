# CMakeLists.txt (root)

cmake_minimum_required(VERSION 3.7.0)

project(cmake-hello-world VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# add_compile_options(-Wall -pedantic -Werror)
add_compile_options(-Wall -pedantic)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;)

enable_testing()


# Add the src subdirectory
add_subdirectory(websocket)
add_subdirectory(src)
add_subdirectory(tests)
