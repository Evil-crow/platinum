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
        ../utility/buffer/buffer.h
        ../utility/buffer/buffer.cc
        ../utility/buffer/writequeue.h
        ../utility/buffer/writequeue.cc
        ../utility/buffer/task.h
        ../utility/buffer/task.cc
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
        ../net/unix_address.h
        ../net/unix_address.cc
        ../net/address.hpp
)
set(
        _PLATINUM_NET
        ${_PLATINUM_SOCKET}
        ../net/acceptor.h
        ../net/acceptor.cc
        ../net/tcp_server.h
        ../net/tcp_server.cc
        ../net/connection.h
        ../net/connection.cc
        ../net/connector.h
        ../net/connector.cc
)

set(
        _PLATINUM_FCGI
        ../protocol/fastCGI/base.h
        ../protocol/fastCGI/component.h
        ../protocol/fastCGI/component.cc
        ../protocol/fastCGI/request_builder.h
        ../protocol/fastCGI/request_builder.cc
        ../protocol/fastCGI/response_parser.h
        ../protocol/fastCGI/response_parser.cc
)
set(
        _PLATINUM_PROTOCOL
        ${_PLATINUM_FCGI}
)

# test file

add_executable(ipaddress ipaddress_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_SOCKET})
target_link_libraries(ipaddress yaml-cpp)

add_executable(reactor_socket reactor_socketapi_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(reactor_socket yaml-cpp)

add_executable(reactor_Socket reactor_socket_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(reactor_Socket yaml-cpp)

add_executable(acceptor acceptor_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(acceptor yaml-cpp)

add_executable(server_connection tcpconnection_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(server_connection yaml-cpp)

add_executable(http_buffer http_buffer_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(http_buffer yaml-cpp)
target_link_libraries(http_buffer httpxx)

add_executable(writequeue writequeue_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET})
target_link_libraries(writequeue yaml-cpp)
target_link_libraries(writequeue httpxx)

add_executable(request_builder request_builder_test.cc
        ${_PLATINUM_PROTOCOL})

add_executable(response_parser response_parser_test.cc
        ${_PLATINUM_PROTOCOL})

add_executable(connector connector_test.cc
        ${_PLATINUM_REACTOR}
        ${_PLATINUJM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET}
        ${_PLATINUM_CONFIG})
target_link_libraries(connector yaml-cpp)
target_link_libraries(connector httpxx)
