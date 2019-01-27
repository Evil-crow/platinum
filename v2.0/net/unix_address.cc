/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  Immplmente Class UnixAddress
 */

#include "unix_address.h"
#include <utility>

using namespace platinum;

UnixAddress::UnixAddress(std::string path)
    : Address(AF_UNIX),
      path_(std::move(path))
{
  addr_ = ToSockaddrUn();
}

void UnixAddress::set_path(std::string &path)
{
  path_ = path;
}

const std::string &UnixAddress::path() const
{
  return path_;
}

sockaddr_un UnixAddress::ToSockaddrUn() const
{
  sockaddr_un addr{};
  ::memset(&addr, 0, sizeof(sockaddr_un));

  auto len = path_.size() > 107 ? 107 : path_.size();
  printf("len: %zd\n", len);
  addr.sun_family = family_;
  ::memcpy(&addr.sun_path, path_.c_str(), len);

  return addr;
}

const sockaddr *UnixAddress::SockaddrPtr() const
{
  return reinterpret_cast<const sockaddr *>(&addr_);
}

const socklen_t UnixAddress::SockaddrLen() const
{
  return sizeof(addr_);
}
