/*
 * Created by Crow on 8/22/18.
 * Description: the implement of class socket
 */


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <iostream>
#include "socket.h"

inline void err_handle(const std::string &msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

PlatinumServer::socket::socket(in_port_t _port)
{
    sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
        err_handle("socket");

    bzero(&sock_sockaddr, sizeof(struct sockaddr_in));
    sock_sockaddr.sin_family = AF_INET;
    sock_sockaddr.sin_port = htons(_port);
    sock_sockaddr.sin_addr.s_addr = htons(INADDR_ANY);
}

PlatinumServer::socket::socket(int sock_fd)
{
    this->sock_fd = sock_fd;
}

PlatinumServer::socket::~socket()
{
    if (this->sock_fd != -1)
        ::close(sock_fd);
    ::bzero(&sock_sockaddr, sizeof(struct sockaddr_in));
}

void PlatinumServer::socket::close()
{
    ::close(this->sock_fd);
    this->sock_fd = -1;
}

bool PlatinumServer::socket::bind()
{
    int ret = ::bind(sock_fd, reinterpret_cast<sockaddr *>(&sock_sockaddr), sizeof(struct sockaddr_in));
    return (ret != -1);
}

bool PlatinumServer::socket::listen()
{
    int ret = ::listen(sock_fd, backlog());
    return (ret != -1);
}

void PlatinumServer::socket::connect()
{
    int buf = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &buf, sizeof(int));
    if (!bind())
        err_handle("bind");
    if (!listen())
        err_handle("listen");
}

int PlatinumServer::socket::accept()
{
    int ret = ::accept(sock_fd, nullptr, nullptr);

    return ret;                    // get the conn_fd by socket class
}
