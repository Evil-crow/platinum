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
        utility/logger/logger.h
        utility/logger/logger.cc
)

set(
        _PLATINUM_SOCKET
        utility/socket/ip_address.h
        utility/socket/ip_address.cc
        utility/socket/socketops.h
        utility/socket/socketops.cc
)

set(
        _PLATINUM_UTILITY
        ${_PLATINUM_LOGGER}
        ${_PLATINUM_SOCKET}
        utility/noncopyable.h
)

set(
        _PLATINUM_CORE
        core/tcp_listen_socket.cc
        core/tcp_listen_socket.h
        core/tcp_connection.h
        core/tcp_connection.cc
)

set(
        EXECUTE_FILE
        platinum.cc
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_CORE}
)

# add sub directory, these directory must have their own CMakeLists.txt
# This means these directory are libraryies
# add_subdirectory(config)

add_executable(Platinum ${EXECUTE_FILE})

# set link libraries
target_link_libraries(Platinum yaml-cpp)