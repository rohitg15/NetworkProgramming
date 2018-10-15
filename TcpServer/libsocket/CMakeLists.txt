set(libsocket_SRCS
    socket.h
    socket.cpp
)

# Declare the library
add_library(libsocket STATIC
    ${libsocket_SRCS}
)

# target_link_libraries(libsocket
#     libfoo
# )

# Specify here the include directories exported
# by this library
target_include_directories(libsocket PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
