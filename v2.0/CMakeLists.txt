cmake_minimum_required(VERSION 3.10)
project(Platinum)

set(PROJECT_VERSION_MAJOR  2)
set(PROJECT_VERSION_MINOR  0)

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_FLAGS -DLOGON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

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
        _PLATINUM_UTILITY
        ${_PLATINUM_LOGGER}
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
        _PLATINUM_NET
        net/tcp_connection.h
        net/tcp_connection.cc
        net/ip_address.h
        net/ip_address.cc
        net/socketops.h
        net/socketops.cc
        net/socket.cc
        net/socket.h
        net/acceptor.cc
        net/acceptor.h
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
# add_subdirectory(config)

add_executable(Platinum ${EXECUTE_FILE})

# set link libraries
target_link_libraries(Platinum yaml-cpp)