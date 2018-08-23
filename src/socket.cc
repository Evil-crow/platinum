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
#include "socket.h"

#define ERR_HANDLE(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

PlatinumServer::socket::socket(int _domain, int _type, in_port_t _port)
{
    sock_domain = _domain;
    sock_domain = _type;
    sock_port = htons(_port);

    sock_fd = ::socket(sock_domain, sock_type, 0);
    if (sock_fd == -1)
        ERR_HANDLE("socket");

    bzero(&sock_sockaddr, sizeof(struct sockaddr_in));
    sock_sockaddr.sin_family = AF_INET;
    sock_sockaddr.sin_port = _port;
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

bool PlatinumServer::socket::_bind()
{
    int ret = ::bind(sock_fd, reinterpret_cast<sockaddr *>(&sock_sockaddr), sizeof(struct sockaddr_in));
    return (ret != -1);
}

bool PlatinumServer::socket::_listen()
{
    int ret = ::listen(sock_fd, backlog());
    return (ret != -1);
}

void PlatinumServer::socket::connect()
{
    if (_bind())
        ERR_HANDLE("bind");
    if (_listen())
        ERR_HANDLE("listen");
}

int PlatinumServer::socket::accept(struct sockaddr_in client)
{
    socklen_t len = sizeof(struct sockaddr_in);

    int ret = ::accept(sock_fd, reinterpret_cast<struct sockaddr *>(&client), &len);
    if (ret == -1)
        ERR_HANDLE("accept");

    return ret;                     // get the conn_fd
}





