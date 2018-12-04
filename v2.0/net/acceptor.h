//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_ACCEPTOR_H
#define PLATINUM_ACCEPTOR_H

#include "tcp_connection.h"
#include "net/socket.h"

namespace platinum {

class TCPConnection;
class Acceptor {
 public:
  explicit Acceptor(in_port_t port);
  Acceptor(in_port_t port, const std::string &ip);
  ~Acceptor() = default;

  void Listening();
  TCPConnection AcceptConnection();

 private:
  Socket listenfd_;
  IPAddress address_;
};

}

#endif //PLATINUM_ACCEPTOR_H
