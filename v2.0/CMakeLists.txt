cmake_minimum_required(VERSION 3.10)
project(platinum)

set(PROJECT_VERSION_MAJOR  2)
set(PROJECT_VERSION_MINOR  0)

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(test)
#set include directories, can avoid UNIX style ".." and "." replace compiler option "-I"
# include_directories(config)
# include_directories(utility)
include_directories(${PROJECT_SOURCE_DIR})

set(
        _PLATINUM_CONFIG
        config/config.h
        config/config.cc
)

set(
        _PLATINUM_LOGGER
        utility/logger.h
        utility/logger.cc
)

set(
        _PLATINUM_BUFFER
        utility/buffer/buffer.h
        utility/buffer/buffer.cc
        utility/buffer/writequeue.h
        utility/buffer/writequeue.cc
        utility/buffer/task.h
        utility/buffer/task.cc
)

set(
        _PLATINUM_UTILITY
        ${_PLATINUM_LOGGER}
        ${_PLATINUM_BUFFER}
        utility/noncopyable.h
)

set(
        _PLATINUM_REACTOR
        reactor/epoller.h
        reactor/epoller.cc
        reactor/event_loop.h
        reactor/event_loop.cc
        reactor/channel.h
        reactor/channel.cc
)

set(
        _PLATINUM_SOCKET
        net/ip_address.cc
        net/ip_address.h
        net/socket.h
        net/socket.cc
        net/socketops.h
        net/socketops.cc
)
set(
        _PLATINUM_NET
        ${_PLATINUM_SOCKET}
        net/acceptor.h
        net/acceptor.cc
        net/tcp_server.h
        net/tcp_server.cc
        net/tcp_connection.h
        net/tcp_connection.cc
)

set(
        EXECUTE_FILE
        platinum.cc
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET}
        ${_PLATINUM_REACTOR}
)

# add sub directory, these directory must have their own CMakeLists.txt
# This means these directory are libraryies

add_executable(platinum ${EXECUTE_FILE})

# set link libraries
target_link_libraries(platinum yaml-cpp)
