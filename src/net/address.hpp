/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the Base class Address
 *         It can be extended to IPAddress / UnixAddress
 */

#ifndef PLATINUM_NET_ADDRESS_HPP
#define PLATINUM_NET_ADDRESS_HPP

#include <sys/socket.h>
#include <sys/types.h>

namespace platinum {

class Address {
 public:
  explicit Address(sa_family_t family) : family_(family) { ; }
  virtual ~Address() = default;

  virtual const sockaddr *SockaddrPtr() const = 0;
  virtual const socklen_t SockaddrLen() const = 0;
 protected:
  sa_family_t family_;
};

}

#endif //PLATINUM_NET_ADDRESS_HPP
