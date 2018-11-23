//
// Created by Crow on 11/22/18.
//

#ifndef PLATINUM_TCP_LISTEN_SOCKET_H
#define PLATINUM_TCP_LISTEN_SOCKET_H

#include "tcp_listen_socket.h"

#include "utility/socket/socketops.h"
#include "utility/noncopyable.h"

namespace platinum {

class IPAddress;
class TCPConnection;

class TCPListenSocket : public Noncopyable {
 public:
  TCPListenSocket();
  TCPListenSocket(const TCPListenSocket &rhs);
  ~TCPListenSocket() override;

  void BindAndListen(const IPAddress &address);
  TCPConnection AcceptConnection();
  int listen_fd();

 private:
  bool SetReusePort(bool on);
  bool SetTcpNoDelay(bool on);
  bool SetKeepAlive(bool on);

  int listen_fd_;
};

}

#endif //PLATINUM_TCP_LISTEN_SOCKET_H
