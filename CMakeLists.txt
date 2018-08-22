# Cmake minium version
cmake_minimum_required(VERSION 3.10)

# Project information
project(PlatinumServer)

# set options
set(CMAKE_CXX_STANDARD 11)

# add include directory
include_directories(include)

# add source directory
aux_source_directory(src/thread_pool SOURCE_THREADPOOL)
aux_source_directory(src/socket SOURCE_SOCKET)

# set files
set(
        INCLUDE_FILES
        include/threadpool/blocked_queue.hpp
        include/threadpool/thread_pool.h
        include/socket/socket.h
)
set(
        SOURCE_FILES
        src/main.cc
        ${SOURCE_THREADPOOL}
        ${SOURCE_SOCKET}
)
set(
        EXECUTABLE_FILES
        ${INCLUDE_FILES}
        ${SOURCE_FILES}
)

# add executable files
add_executable(PlatinumServer ${EXECUTABLE_FILES})

# add dynamic link libarary
link_directories(/usr/lib64)
target_link_libraries(PlatinumServer curl)
target_link_libraries(PlatinumServer pthread)