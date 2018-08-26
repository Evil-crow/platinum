/*
 * Created by Crow on 8/24/18.
 * Description: This file is so important in PlatinumServer, It's used to deal with the HTTP affair
 *          It bases on <httpxx/Request.hpp>, <httpxx/ResponseBuilder.hpp> and "http_response.h"
 *          to deal with the HTTP-affair, by parse the recv-data and build-response
 */

#include <string>
#include <sys/socket.h>
#include <httpxx/Request.hpp>
#include <iostream>
#include <cstring>
#include "http_affair.h"
#include "http_response.h"

void deal_http_affair(PlatinumServer::socket &conn_socket, PlatinumServer::epoll &epollobj)
{
    std::cout << "get in the deal_http_affair" << std::endl;
    char buffer[100];
    http::Request request;

    // get the HTTP Request
    while (!request.complete()) {
        const ssize_t transferred = ::recv(conn_socket.get_fd(), buffer, 100, 0);
        std::cout << "recv_strart: " << std::endl << buffer << "recv end" << std::endl << std::endl;
        if (transferred == 0)
            break;
        request.feed(buffer, (size_t)transferred);
        std::memset(buffer, 0, 100);
    }

    std::cout << "feed succeed" << std::endl;

    // validate the request really completely be got
    if (!request.complete()) {
        std::cout << "Error, Not get the complete Request" << std::endl;
        return ;
    }

    // Display the Request
    std::cout << "xxxxxxxxx Completely Start xxxxxxxxxxxx" << std::endl
            << "method: " << request.method().to_string() << std::endl
            << "Url: " << request.url() << std::endl
            << "Headers: " << std::endl
            << "Connection: " << request.header("Connection") << std::endl
            << "Host: " << request.header("Host") << std::endl
            << "Body: " << std::endl
            << "xxxxxxxxx Completely end xxxxxxxxxxxxxx" << std::endl;
    request.clear();


    PlatinumServer::http_response response;
    response.set_status(200);
    response.set_major_version(1);
    response.set_minor_version(2);
    response.set_header("Server", "PlatinumServer/1.0");
    response.set_header("Connection", "close");

    response.build();
    std::string res = response.get_response();
    ::send(conn_socket.get_fd(), res.c_str(), res.size(), 0);
    std::cout << "get out!" << std::endl;

    epollobj.modify(conn_socket.get_fd(), { EPOLLIN, EPOLLONESHOT });
}



