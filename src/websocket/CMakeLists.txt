set(libwebsockets_lib "/usr/lib/aarch64-linux-gnu/libwebsockets.so.17")
message(STATUS "Using libwebsockets: ${libwebsockets_lib}")

# Debug output
if(libwebsockets_lib)
    message(STATUS "Found libwebsockets: ${libwebsockets_lib}")
else()
    message(FATAL_ERROR "libwebsockets not found!")
endif()

# Create a library
add_library(websocket websocket.cpp ${libwebsockets_lib})
