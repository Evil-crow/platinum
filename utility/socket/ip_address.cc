//
// Created by Crow on 11/22/18.
//

#include "ip_address.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utility/logger/logger.h"

using namespace platinum;

IPAddress::IPAddress(in_port_t port)
      : family_(AF_INET), port_(port)
{
  in_addr temp{INADDR_ANY};
  auto str = ::inet_ntoa(temp);
  ip_ = std::string(str);
}

explicit IPAddress::IPAddress(sockaddr_in sockaddr)
      : family_(sockaddr.sin_family),
        port_(::ntohs(sockaddr.sin_port))
{
    in_addr temp{sockaddr.sin_addr};
    auto str = ::inet_ntoa(temp);
    ip_ = std::string(str);
}

IPAddress::IPAddress(sa_family_t family, in_port_t port)
      : family_(family), port_(port)
{
    in_addr temp{INADDR_ANY};
    auto str = ::inet_ntoa(temp);
    ip_ = std::string(str);
}


sockaddr_in IPAddress::ToSockaddrIn() const
{
  sockaddr_in sockaddr_{};

  sockaddr_.sin_family = family_;
  sockaddr_.sin_port = ::htons(port_);
  const char *str = ip_.c_str();
  if (::inet_pton(family_, str, &sockaddr_.sin_port)) {
    LOG(ERROR) << "IPAddress::ToSockaddrIn -> ::inet_pton";
    std::abort();
  }

  return sockaddr_;
}

inline const in_port_t &IPAddress::port() const
{
  return port_;
}

inline const std::string &IPAddress::ip() const
{
  return ip_;
}

