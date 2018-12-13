cmake_minimum_required(VERSION 3.10)
project(Test)

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(../)

set(
        _PLATINUM_CONFIG
        ../config/config.h
        ../config/config.cc
)

set(
        _PLATINUM_LOGGER
        ../utility/logger.h
        ../utility/logger.cc
)

set(
        _PLATINUM_UTILITY
        ${_PLATINUM_LOGGER}
        ../utility/noncopyable.h
)

set(
        _PLATINUM_REACTOR
        ../reactor/epoller.h
        ../reactor/epoller.cc
        ../reactor/event_loop.h
        ../reactor/event_loop.cc
        ../reactor/channel.h
        ../reactor/channel.cc
)

set(
        _PLATINUM_SOCKET
        ../net/ip_address.cc
        ../net/ip_address.h
        ../net/socket.h
        ../net/socket.cc
        ../net/socketops.h
        ../net/socketops.cc
)
set(
        _PLATINUM_NET
        ${_PLATINUM_SOCKET}
        ../net/acceptor.h
        ../net/acceptor.cc
        ../net/tcp_server.h
        ../net/tcp_server.cc
        ../net/tcp_connection.h
        ../net/tcp_connection.cc
)

# test file

add_executable(IPAddress_test IPAddress_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_SOCKET})
target_link_libraries(IPAddress_test yaml-cpp)

add_executable(acceptor_test acceptor_test.cc
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_NET}
        ${_PLATINUM_REACTOR})
target_link_libraries(acceptor_test yaml-cpp)

add_executable(reactor_socket reactor_socket.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(reactor_socket yaml-cpp)

add_executable(reactor_Socket reactor_Socket.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(reactor_Socket yaml-cpp)

add_executable(reactor_Acceptor reactor_Acceptor.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(reactor_Acceptor yaml-cpp)

add_executable(tcp_server_connection tcp_server_connection.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(tcp_server_connection yaml-cpp)
