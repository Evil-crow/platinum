//
// Created by Crow on 11/23/18.
//

#ifndef PLATINUM_TCP_CONNECTION_H
#define PLATINUM_TCP_CONNECTION_H

#include "core/channel.h"
#include "utility/noncopyable.h"
#include "utility/socket/ip_address.h"


namespace platinum {

class IPAddress;
class TCPChannel;
class Acceptor;
class TCPConnection : public Noncopyable {
  friend class Acceptor;
 public:
  TCPConnection(const TCPConnection &) = delete;
  TCPConnection operator=(const TCPConnection &) = delete;
  IPAddress GetHostInfo();
  IPAddress GetPeerInfo();


 private:
  TCPConnection(int connect_fd);
  ~TCPConnection() override;

  int connfd_;
  TCPChannel channel_;
  IPAddress hostinfo_;
  IPAddress peerinfo_;
};

}

#endif //PLATINUM_TCP_CONNECTION_H
