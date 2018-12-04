//
// Created by Crow on 11/25/18.
//

#include <netinet/tcp.h>

#include "utility/logger.h"
#include "socket.h"

using namespace platinum;

Socket::Socket() : sockfd_(socket::Socket())
{
  ;
}

Socket::Socket(int sockfd) : sockfd_(sockfd)
{
  ;
}


Socket::~Socket()
{
  socket::Close(sockfd_);
}

void Socket::Bind(const IPAddress &address)
{
  sockaddr_in sockaddr = address.ToSockaddrIn();
  socket::BindOrDie(sockfd_, socket::SockaddrCast(&sockaddr));
}

void Socket::Listen()
{
  socket::ListenOrDie(sockfd_, SOMAXCONN);
}

int Socket::Accept()
{
  int connfd = socket::Accept(sockfd_);
  return connfd;
}

bool Socket::SetReusePort(bool on)
{
  int val = on ? 1 : 0;

  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
      &val, static_cast<socklen_t>(sizeof(val)));

  if (ret < 0 && on) {
    LOG(ERROR) << "Socket::SetReusePort";
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
    LOG(ERROR) << "Socket::SetKeepAlive";
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
    LOG(ERROR) << "Socket::SetTcpNoDelay";
    std::abort();
  }

  return true;
}

