set(libwebsockets_lib "/usr/lib/aarch64-linux-gnu/libwebsockets.so.17")

if(NOT EXISTS libwebsockets_lib)
    set(libwebsockets_lib "/usr/lib/x86_64-linux-gnu/libwebsockets.so.17")
endif()

# Create a library
add_library(websocket websocket.cpp ${libwebsockets_lib})
