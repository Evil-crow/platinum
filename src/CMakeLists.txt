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
        config/config.h config/config.cc
)
set(
        _PLATINUM_LOGGER
        utility/logger.h utility/logger.cc
)
set(
        _PLATINUM_BUFFER
        utility/buffer/buffer.h utility/buffer/buffer.cc
        utility/buffer/writequeue.h utility/buffer/writequeue.cc
        utility/buffer/task.h utility/buffer/task.cc
)
set(
        _PLATINUM_FILE
        utility/dir.h utility/dir.cc
        utility/file_reader.h utility/file_reader.cc
)
set(
        _PLATINUM_UTILITY
        ${_PLATINUM_LOGGER}
        ${_PLATINUM_BUFFER}
        ${_PLATINUM_FILE}
        utility/noncopyable.h
)
set(
        _PLATINUM_REACTOR
        reactor/epoller.h reactor/epoller.cc
        reactor/event_loop.h reactor/event_loop.cc
        reactor/channel.h reactor/channel.cc
)
set(
        _PLATINUM_ADDRESS
        net/address.hpp
        net/ip_address.h net/ip_address.cc
        net/unix_address.cc net/unix_address.h
)
set(
        _PLATINUM_SOCKET
        net/socket.h net/socket.cc
        net/socketops.h net/socketops.cc
)
set(
        _PLATINUM_NET
        ${_PLATINUM_SOCKET}
        ${_PLATINUM_ADDRESS}
        net/acceptor.h net/acceptor.cc
        net/tcp_server.h net/tcp_server.cc
        net/connection.h net/connection.cc
        net/connector.cc net/connector.h
)
set(
        _PLATINUM_HTTP
        protocol/http/base.h
        protocol/http/request.h protocol/http/request.cc
        protocol/http/request_parser.cc protocol/http/request_parser.h
        protocol/http/response.h protocol/http/response.cc
        protocol/http/response_builder.cc protocol/http/response_builder.h
)
set(
        _PLATINUM_FCGI
        protocol/fastCGI/base.h
        protocol/fastCGI/component.h protocol/fastCGI/component.cc
        protocol/fastCGI/request_builder.h protocol/fastCGI/request_builder.cc
        protocol/fastCGI/response_parser.h protocol/fastCGI/response_parser.cc
)
set(
        _PLATINUM_PROTOCOL
        ${_PLATINUM_FCGI}
        ${_PLATINUM_HTTP}
        protocol/parser.hpp
)
set(
        _PLATINUM_HANDLER
        core/handler/handler.hpp
        core/handler/static_handler.h core/handler/static_handler.cc
        core/handler/fcgi_handler.h core/handler/fcgi_handler.cc
)
set(
        _PLATINUM_CORE
        core/platinum.h core/platinum.cc
        core/affair.h core/affair.cc
        ${_PLATINUM_HANDLER}
)
set(
        EXECUTE_FILE
        main.cc
        ${_PLATINUM_CORE}
        ${_PLATINUM_CONFIG}
        ${_PLATINUM_UTILITY}
        ${_PLATINUM_NET}
        ${_PLATINUM_REACTOR}
        ${_PLATINUM_PROTOCOL}
)

# add sub directory, these directory must have their own CMakeLists.txt
# This means these directory are libraryies

add_executable(platinum ${EXECUTE_FILE})

# set link libraries
target_link_libraries(platinum yaml-cpp)
target_link_libraries(platinum pthread)
