/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe Class Connector
 *         Which can use IPAddrss / UnixAddress to connect peer endpoint
 */

#ifndef PLATINUM_NET_CONNECTOR_H
#define PLATINUM_NET_CONNECTOR_H

#include <memory>
#include <functional>
#include "protocol/parser.hpp"

#include "net/socket.h"

namespace platinum {

class Buffer;
class EventLoop;
class Connection;
class UnixAddress;
class Connector {
 public:
  using MessageCallback = std::function<long (Connection *, Buffer &, std::unique_ptr<platinum::Parser> &)>;
  using WriteableCallback = std::function<void()>;

  Connector(EventLoop *loop, const IPAddress &address, ParserType type);
  Connector(EventLoop *loop, const UnixAddress &address, ParserType type);
  ~Connector() = default;

  void StartNewConnection();
  void SendData(const unsigned char *data, size_t total);
  void SendFile(const std::string &pathname, size_t total);
  void ShutdownConnection();

  // copy TCPServer callback -> TCPConnection, can't move, we'll create much TCPConnection by TCPServer::*callback
  void SetMessageCallback(const MessageCallback &callback);
  void SetWriteCallback(const WriteableCallback &callback);
  const MessageCallback &message_callback() const;
  const WriteableCallback &writeable_callback() const;

  int fd() const { return fd_; }
  int forward_fd() const { return forward_fd_; }
  void set_forward_fd(int fd) { forward_fd_ = fd; }
  auto connection_ptr() const -> std::shared_ptr<Connection>;

 private:
  void HandleEvent();

  int fd_;                                                                     // Not RAII Handle, only store the fd, resource managment to TCPConnection
  int forward_fd_;
  EventLoop *loop_;                                                            // use loop to control resource, like AddChannel()
  std::unique_ptr<Address> address_;                                           // Address, Bind with this
  std::shared_ptr<Connection> connection_ptr_;
  WriteableCallback write_callback_;
  MessageCallback message_callback_;
};

}

#endif //PLATINUM_NET_CONNECTOR_H
