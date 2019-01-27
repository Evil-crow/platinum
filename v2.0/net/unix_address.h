/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the Derived Class UnixAddress
 */

#ifndef PLATINUM_UNIX_ADDRESS_H
#define PLATINUM_UNIX_ADDRESS_H

#include <sys/un.h>
#include <string>

#include "net/address.hpp"

namespace platinum {

class UnixAddress : public Address {
 public:
  UnixAddress() : Address(AF_UNIX) { ; }
  ~UnixAddress() override = default;
  UnixAddress(const UnixAddress &addr) = default;
  UnixAddress &operator=(const UnixAddress &addr) = default;

  explicit UnixAddress(std::string path);
  void set_path(std::string &path);
  const std::string &path() const;

  const sockaddr *SockaddrPtr() const override;
  const socklen_t SockaddrLen() const override;

 private:
  sockaddr_un ToSockaddrUn() const;

  std::string path_;
  sockaddr_un addr_{};
};

}

#endif //PLATINUM_UNIX_ADDRESS_H
