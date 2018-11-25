//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_ACCEPTOR_H
#define PLATINUM_ACCEPTOR_H

#include "core/tcp_connection.h"
#include "utility/socket/socket.h"

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
