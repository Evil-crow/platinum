//
// Created by Crow on 11/21/18.
//

#include "socketops.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include "utility/logger/logger.h"

using namespace platinum;

int socket::Socket()
{
  int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if (fd < 0) {
    LOG(ERROR) << "socket::Socket";
    std::abort();
  }

  return fd;
}

bool socket::BindOrDie(int sockfd, const struct sockaddr *addr)
{
  if (::bind(sockfd, addr, sizeof(addr))) {
    LOG(ERROR) << "socket::BindOrDie";
    std::abort();
  }

  return true;
}

bool socket::ListenOrDie(int sockfd, int backlog)
{
  if (::listen(sockfd, backlog)) {
    LOG(ERROR) << "socket::Listen";
    std::abort();
  }

  return true;
}

int socket::Accept(int sockfd)
{
  int connfd = ::accept4(sockfd, nullptr, nullptr, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (connfd < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      LOG(INFO) << "socket: accept (Maybe Non-block socket)";
    } else {
      LOG(INFO) << "socket::accept";
      std::abort();
    }
  } else {
    return connfd;
  }
}

sockaddr_in socket::GetSockName(int sockfd)
{
  sockaddr_in sockaddr{};
  socklen_t len = sizeof(sockaddr);
  if (::getsockname(sockfd, reinterpret_cast<sockaddr *>(&sockaddr), &len)) {
    LOG(ERROR) << "socket::GetSockName";
    std::abort();
  }

  return sockaddr;
}

sockaddr_in socket::GetPeerName(int sockfd)
{
  sockaddr_in sockaddr{};
  socklen_t len = sizeof(sockaddr);
  if (::getsockname(sockfd, reinterpret_cast<sockaddr *>(&sockaddr), &len)) {
    LOG(ERROR) << "socket::GetSockName";
    std::abort();
  }

  return sockaddr;
}

sockaddr *socket::SockaddrCast(sockaddr_in *sockaddr_)
{
  return reinterpret_cast<sockaddr *>(sockaddr_);
}

bool socket::Close(int sockfd)
{
  if (::close(sockfd)) {
    LOG(ERROR) << "Close Error";
    std::abort();
  }
  return true;
}

bool socket::ShutdownWrite(int sockfd)
{
  if (::shutdown(sockfd, SHUT_WR)) {
    LOG(ERROR) << "socket::ShutdownWrite";
    std::abort();
  }
}
#pragma clang diagnostic pop