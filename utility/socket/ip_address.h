//
// Created by Crow on 11/22/18.
//

#ifndef PLATINUM_IP_ADDRESS_H
#define PLATINUM_IP_ADDRESS_H

#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <utility>

namespace platinum {

class IPAddress {
 public:
  IPAddress(sa_family_t family, in_port_t port, std::string ip)
      : family_(family), port_(port), ip_(std::move(ip)) {}
  IPAddress(sa_family_t family, in_port_t port, const char *p)
      : family_(family), port_(port), ip_(p) {}
  IPAddress(const IPAddress &rhs) = default;
  const in_port_t &port() const;
  const std::string &ip() const;
  sockaddr_in ToSockaddrIn() const;

 private:
  sa_family_t family_;
  in_port_t port_;
  std::string ip_;
};

}

#endif //PLATINUM_IP_ADDRESS_H
