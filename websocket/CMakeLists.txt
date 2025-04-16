find_library(websocketslib1 uwebsockets PATHS ./uWebSockets NO_DEFAULT_PATH)
add_executable(mytest mytest.cpp)
target_link_libraries(mytest ${websocketslib1})

