# Cmake minium version
cmake_minimum_required(VERSION 3.10)

# Project information
project(PlatinumServer)

# set options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -lpthread)

# add include directory
include_directories(include)

# add source directory
aux_source_directory(src/thread_pool SOURCE_THREADPOOL)

# add executable files
add_executable(PlatinumServer  src/main.cc include/threadpool/blocked_queue.hpp include/threadpool/thread_pool.h ${SOURCE_THREADPOOL})