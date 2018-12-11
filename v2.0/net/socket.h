//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_SOCKET_H
#define PLATINUM_SOCKET_H

#include "ip_address.h"
#include "socketops.h"

namespace platinum {

class IPAddress;
class Socket {
 public:
  Socket();
  explicit Socket(int sockfd);
  Socket(const Socket &) = default;
  Socket &operator=(const Socket &) = default;
  Socket(const Socket &&) noexcept ;
  Socket &operator=(const Socket &&) noexcept ;
  ~Socket();

  void Bind(const IPAddress &address);
  void Listen();
  int Accept(IPAddress &address);

  bool SetReusePort(bool on);
  bool SetTcpNoDelay(bool on);
  bool SetKeepAlive(bool on);

  int fd() { return sockfd_; }
 private:
  int sockfd_;
};

}

#endif //PLATINUM_SOCKET_H
