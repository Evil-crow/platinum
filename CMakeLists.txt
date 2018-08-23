# Cmake minium version
cmake_minimum_required(VERSION 3.10)

# Project information
project(PlatinumServer)

# set options
set(CMAKE_CXX_STANDARD 11)

# add include directory
include_directories(include)

# set files
set(
        INCLUDE_FILES
        include/blocked_queue.hpp
        include/thread_pool.h
        include/socket.h
        include/epoll.h
        include/http_response.h
)
set(
        SOURCE_FILES
        src/main.cc
        src/socket.cc
        src/epoll.cc
        src/thread_pool.cc
        src/http_response.cc
        src/http_affair.cc
)
set(
        EXECUTABLE_FILES
        ${INCLUDE_FILES}
        ${SOURCE_FILES}
)

# add executable files
add_executable(PlatinumServer ${EXECUTABLE_FILES})

# add link libarary
link_libraries(pthread)      # Dynamic library
link_libraries(httpxx)       # Static  library