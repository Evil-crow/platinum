//
// Created by Crow on 11/22/18.
//

#include "ip_address.h"

#include "utility/logger/logger.h"

using namespace platinum;

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
