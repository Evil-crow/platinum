//
// Created by Crow on 8/22/18.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <stdexcept>
#include "socket/socket.h"

socket::socket(int _domain, int _type, in_port_t _port)
{
    sock_domain = _domain;
    sock_domain = _type;
    sock_port = htons(_port);

    sock_fd = ::socket(sock_domain, sock_type, 0);
    if (sock_fd == -1)
        std::runtime_error("socket get error!");

    bzero(&sock_sockaddr, sizeof(struct sockaddr_in));
    sock_sockaddr.sin_family = AF_INET;
    sock_sockaddr.sin_port = _port;
    sock_sockaddr.sin_addr.s_addr = htons(INADDR_ANY);
}

bool socket::_bind()
{
    int ret = ::bind(sock_fd,
                     reinterpret_cast<sockaddr *>(&sock_sockaddr),
                 sizeof(struct sockaddr_in));
    return (ret != -1);
}

bool socket::_listen()
{
    int ret = ::listen(sock_fd, backlog());
    return (ret != -1);
}

void socket::connect()
{
    if (_bind())
        std::runtime_error("bind error");
    if (_listen())
        std::runtime_error("listen error");
}

inline int socket::get_fd()
{
    return sock_fd;
}

int socket::accept(struct sockaddr_in client)
{
    socklen_t len = sizeof(struct sockaddr_in);

    int ret = ::accept(sock_fd, reinterpret_cast<struct sockaddr *>(&client), &len);
    if (ret == -1)
        std::runtime_error("accept error");

    return ret;                     // get the conn_fd
}

socket::~socket()
{
    ::bzero(&sock_sockaddr, sizeof(struct sockaddr_in));
    ::close(sock_fd);
}



