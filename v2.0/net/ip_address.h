//
// Created by Crow on 11/22/18.
//

#ifndef PLATINUM_IP_ADDRESS_H
#define PLATINUM_IP_ADDRESS_H

#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <utility>

#include "net/address.hpp"

namespace platinum {

class IPAddress : public Address {
 public:
  IPAddress() : Address(AF_INET) { ; }
  ~IPAddress() override = default;

  explicit IPAddress(in_port_t port);
  explicit IPAddress(sockaddr_in sockaddr);
  IPAddress(const IPAddress &&address) noexcept;
  IPAddress &operator=(IPAddress &&address) noexcept;

  IPAddress(const IPAddress &rhs) = default;
  IPAddress(in_port_t port, std::string ip)
      : Address(AF_INET),
        port_(port),
        ip_(std::move(ip))
  {
     addr_ = ToSockaddrIn();
  }

  void set_port(in_port_t port);
  void set_ip(const std::string &str);
  const in_port_t &port() const;
  const std::string &ip() const;
  const sockaddr *SockaddrPtr() const override;

 private:
  sockaddr_in ToSockaddrIn() const;

  in_port_t port_{};
  std::string ip_;
  sockaddr_in addr_{};
};

}

#endif //PLATINUM_IP_ADDRESS_H
