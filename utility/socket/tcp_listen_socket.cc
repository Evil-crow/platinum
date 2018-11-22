//
// Created by Crow on 11/22/18.
//

#include "tcp_listen_socket.h"

#include "socketops.h"

using namespace platinum;

TCPListenSocket::TCPListenSocket()
    : listen_fd_(socket::Socket())
{
  ;
}

TCPListenSocket::TCPListenSocket(const TCPListenSocket &rhs)
    : listen_fd_(rhs.listen_fd_)
{
  ;
}

TCPListenSocket::~TCPListenSocket() :
{
  socket::Close(this->listen_fd_);
}

void TCPListenSocket::Bind()
{
  socket::BindOrDie(this->listen_fd_, )
}
