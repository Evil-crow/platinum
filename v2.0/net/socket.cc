//
// Created by Crow on 11/25/18.
//

#include "net/socket.h"

#include <netinet/tcp.h>

#include "utility/logger.h"

using namespace platinum;

Socket::Socket(socket::Type type) : sockfd_(socket::Socket(type))
{
  ;
}

Socket::Socket(int sockfd) : sockfd_(sockfd)
{
  ;
}

Socket::Socket(const Socket &&socket) noexcept
{
  sockfd_ = socket.sockfd_;
}

Socket& Socket::operator=(Socket &&socket) noexcept
{
  sockfd_ = socket.sockfd_;
  return *this;
}

Socket::~Socket()
{
  socket::Close(sockfd_);
}

void Socket::Bind(const Address &address)
{
  socket::BindOrDie(sockfd_, address.SockaddrPtr());
}

void Socket::Listen()
{
  socket::ListenOrDie(sockfd_, SOMAXCONN);
}

int Socket::Accept(IPAddress &address)
{
  int connfd = socket::Accept(sockfd_, address);
  return connfd;
}

bool Socket::SetReusePort(bool on)
{
  int val = on ? 1 : 0;

  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
      &val, static_cast<socklen_t>(sizeof(val)));

  if (ret < 0 && on) {
    LOG(ERROR) << "Socket::SetReusePort()";
    std::abort();
  }

  return true;
}

bool Socket::SetKeepAlive(bool on)
{
  int val = on ? 1 : 0;

  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
      &val, static_cast<socklen_t>(sizeof(val)));

  if (ret < 0 && on) {
    LOG(ERROR) << "Socket::SetKeepAlive()";
    std::abort();
  }

  return true;
}

bool Socket::SetTcpNoDelay(bool on)
{
  int val = on ? 1 : 0;

  int ret = ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
      &val, static_cast<socklen_t>(sizeof(val)));

  if (ret < 0 && val) {
    LOG(ERROR) << "Socket::SetTcpNoDelay()";
    std::abort();
  }

  return true;
}

