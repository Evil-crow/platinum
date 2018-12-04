//
// Created by Crow on 11/23/18.
//

#ifndef PLATINUM_TCP_CONNECTION_H
#define PLATINUM_TCP_CONNECTION_H

#include "reactor/channel.h"
#include "utility/noncopyable.h"
#include "net/ip_address.h"


namespace platinum {

class Socket;
class Acceptor;
class IPAddress;
class TCPChannel;
class TCPConnection : public Noncopyable {
  friend class Acceptor;
 public:
  TCPConnection(const TCPConnection &) = delete;
  TCPConnection operator=(const TCPConnection &) = delete;
  IPAddress GetHostInfo() const { return hostinfo_; }
  IPAddress GetPeerInfo() const { return peerinfo_; }

 private:
  TCPConnection(int connect_fd);
  ~TCPConnection() override;

  Socket connfd_;
  TCPChannel channel_;
  IPAddress hostinfo_;
  IPAddress peerinfo_;
};

}

#endif //PLATINUM_TCP_CONNECTION_H
