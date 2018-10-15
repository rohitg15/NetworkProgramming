set(libserver_SRCS
    server.h
    server.cpp
)

# Declare the library
add_library(libserver STATIC
    ${libserver_SRCS}
)

target_link_libraries(libserver
    libsocket
)

# Specify here the include directories exported
# by this library
target_include_directories(libserver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
