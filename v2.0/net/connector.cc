/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief
 */

#include "connector.h"

#include "net/ip_address.h"
#include "net/unix_address.h"

using namespace platinum;

Connector::Connector(EventLoop *loop, const IPAddress &address)
    : loop_(loop),
      connfd_(socket::Socket(socket::INET)),
      address_(std::make_unique<IPAddress>(address)),
      connection_ptr_(nullptr)
{
  ;
}

Connector::Connector(EventLoop *loop, const UnixAddress &address)
    : loop_(loop),
      connfd_(socket::Socket(socket::UNIX)),
      address_(std::make_unique<UnixAddress>(address)),
      connection_ptr_(nullptr)
{
  ;
}

