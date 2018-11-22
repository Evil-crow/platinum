//
// Created by Crow on 11/22/18.
//

#ifndef PLATINUM_TCP_LISTEN_SOCKET_H
#define PLATINUM_TCP_LISTEN_SOCKET_H

#include "tcp_listen_socket.h"

#include "socketops.h"
#include "noncopyable.h"

namespace platinum {

class IPAddres;
class TCPConnection;

class TCPListenSocket : public Noncopyable {
 public:
  TCPListenSocket();
  TCPListenSocket(const TCPListenSocket &rhs);
  ~TCPListenSocket() override;

  void Bind(IPAddres &address);
  void Listen();
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
