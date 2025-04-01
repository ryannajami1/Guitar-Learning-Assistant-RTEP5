set(libwebsockets_lib "/usr/lib/aarch64-linux-gnu/libwebsockets.so.17")
message(STATUS "Using libwebsockets: ${libwebsockets_lib}")

# Debug output
if(libwebsockets_lib)
    message(STATUS "Found libwebsockets: ${libwebsockets_lib}")
else()
    message(FATAL_ERROR "libwebsockets not found!")
endif()

# Create an executable
add_executable(mytest mytest.cpp)

add_executable(echotest echotest.cpp)

target_link_libraries(echotest ${libwebsockets_lib})

# Link the library
target_link_libraries(mytest ${libwebsockets_lib})

