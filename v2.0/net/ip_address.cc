//
// Created by Crow on 11/22/18.
//

#include "ip_address.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "utility/logger.h"

using namespace platinum;

IPAddress::IPAddress(in_port_t port)
      : family_(AF_INET), port_(port)
{
  in_addr temp{INADDR_ANY};
  auto str = ::inet_ntoa(temp);
  ip_ = std::string(str);
}

IPAddress::IPAddress(sockaddr_in sockaddr)
      : family_(sockaddr.sin_family),
        port_(::ntohs(sockaddr.sin_port))
{
    in_addr temp{sockaddr.sin_addr};
    auto str = ::inet_ntoa(temp);
    ip_ = std::string(str);
}

sockaddr_in IPAddress::ToSockaddrIn() const
{
  sockaddr_in sockaddr_{};

  sockaddr_.sin_family = family_;
  sockaddr_.sin_port = ::htons(port_);
  const char *str = ip_.c_str();
  if (::inet_pton(family_, str, &sockaddr_.sin_addr) < 1) {
    LOG(ERROR) << "IPAddress::ToSockaddrIn -> ::inet_pton";
    std::abort();
  }

  return sockaddr_;
}

const in_port_t &IPAddress::port() const
{
  return port_;
}

const std::string &IPAddress::ip() const
{
  return ip_;
}

