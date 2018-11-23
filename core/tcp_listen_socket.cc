//
// Created by Crow on 11/22/18.
//

#include "tcp_listen_socket.h"

#include <netinet/tcp.h>

#include "utility/socket/socketops.h"
#include "utility/socket/ip_address.h"
#include ""
#include "utility/logger/logger.h"

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

void TCPListenSocket::BindAndListen(const IPAddress &address)
{
  auto sockaddr_ = address.ToSockaddrIn();
  socket::BindOrDie(listen_fd_, socket::SockaddrCast(&sockaddr_));
  socket::ListenOrDie(listen_fd_, SOMAXCONN);
}

TCPConnection TCPListenSocket::AcceptConnection()
{
  return TCPConnection();
}

inline int TCPListenSocket::listen_fd()
{
  return listen_fd_;
}

bool TCPListenSocket::SetReusePort(bool on)
{
  int val = on ? 1 : 0;

  auto ret = ::setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEPORT, &val, static_cast<socklen_t>(sizeof(val)));
  if (ret < 0 && on) {
    LOG(ERROR) << "TCPListenSocket::SetReusePort";
    std::abort();
  }

  return true;
}

bool TCPListenSocket::SetKeepAlive(bool on)
{
  int val = on ? 1 : 0;

  auto ret = ::setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEPORT, &val, static_cast<socklen_t>(sizeof(val)));
  if (ret < 0 && on) {
    LOG(ERROR) << "TCPListenSocket::SetKeepLive";
    std::abort();
  }

  return true;
}

bool TCPListenSocket::SetTcpNoDelay(bool on)
{
  int val = on ? 1 : 0;

  auto ret = ::setsockopt(listen_fd_, IPPROTO_TCP, TCP_NODELAY, &val, static_cast<socklen_t>(sizeof(val)));
  if (ret < 0 && on) {
    LOG(ERROR) << "TCPListenSocket::SetTcpNoDelay";
    std::abort();
  }

  return true;
}


