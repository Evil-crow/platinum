cmake_minimum_required(VERSION 3.10)

project(Platinum)

set(PROJECT_VERSION_MAJOR  2)
set(PROJECT_VERSION_MINOR  0)

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_FLAGS -DLOGOFF)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#set include directories, can avoid UNIX style ".." and "." replace compiler option "-I"
include_directories(config)
include_directories(utility)

set(
        _PLATINUM_CONFIG
        config/config.h
        config/config.cc
)

set(
        _PLATINUM_SOCKET
        utility/socket.h
        utility/socket.cc
)

set(
        _PLATINUM_LOG
        utility/log.hpp
)

set(
        EXECUTE_FILE
        platinum.cc
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_SOCKET}
        ${_PLATINUM_LOG}
)

# add sub directory, these directory must have their own CMakeLists.txt
# This means these directory are libraryies
# add_subdirectory(config)

add_executable(Platinum ${EXECUTE_FILE})

# set link libraries
target_link_libraries(Platinum yaml-cpp)