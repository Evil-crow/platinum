/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief This file is to immplmente Class Connector
 *        Use platinum::GetCurrentServer to operate.
 */

#include <iostream>
#include <utility/logger.h>
#include "connector.h"

#include "net/ip_address.h"
#include "net/unix_address.h"
#include "net/connection.h"
#include "net/tcp_server.h"

using namespace platinum;

Connector::Connector(EventLoop *loop, const IPAddress &address, ParserType type)
    : loop_(loop),
      fd_(socket::Socket(socket::INET)),
      address_(std::make_unique<IPAddress>(address)),
      connection_ptr_(std::make_shared<Connection>(loop_, fd_, type))
{
  ;
}

Connector::Connector(EventLoop *loop, const UnixAddress &address, ParserType type)
    : loop_(loop),
      fd_(socket::Socket(socket::UNIX)),
      address_(std::make_unique<UnixAddress>(address)),
      connection_ptr_(std::make_shared<Connection>(loop_, fd_, type))
{
  ;
}

void Connector::SendData(const unsigned char *data, size_t total)
{
  connection_ptr_->SendData(data, total);
}

void Connector::SendFile(const std::string &pathname, size_t total)
{
  connection_ptr_->SendFile(pathname, total);
}
void Connector::ShutdownConnection()
{
  connection_ptr_->ShutDownConnection();
}

void Connector::StartNewConnection()
{
  ::connect(fd_, address_->SockaddrPtr(), address_->SockaddrLen());
  platinum::TcpServer::NewConnection(this);
}

void Connector::HandleEvent()
{
  int error{};
  socklen_t len(sizeof(int));
  int ret = ::getsockopt(fd_, SOL_SOCKET, SO_ERROR, &error, &len);
  if (ret < 0) {
    LOG(ERROR) << "Connector::HandleWrite() => ::getsockopt() Error";
    std::abort();
  }

  if (error) {
    switch (error) {
      case EAGAIN:
      case EINPROGRESS:
        break;
      default:
        LOG(ERROR) << "Connector::HandleWrite() => Connect Error";
        platinum::TcpServer::ForceClose(fd_);
    }
  }
  // TODO: Add selfconnect check out + reconnect mechanism
}

void Connector::SetMessageCallback(const Connector::MessageCallback &callback)
{
  message_callback_ = callback;
}

void Connector::SetWriteCallback(const Connector::WriteableCallback &callback)
{
  write_callback_ = [=](){
    printf("OK\n");
    HandleEvent();
    callback();
  };
}

const Connector::MessageCallback &Connector::message_callback() const
{
  return message_callback_;
}

const Connector::WriteableCallback &Connector::writeable_callback() const
{
  return write_callback_;
}

auto Connector::connection_ptr() const -> std::shared_ptr<Connection>
{
  return connection_ptr_;
}