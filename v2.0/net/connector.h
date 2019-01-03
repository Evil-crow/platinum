/**
 * Created by Crow on 12/28/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe Class Connector
 *         Which can use IPAddrss / UnixAddress to connect peer endpoint
 */

#ifndef PLATINUM_CONNECTOR_H
#define PLATINUM_CONNECTOR_H

#include <memory>
#include <functional>

#include "net/socket.h"

namespace platinum {

class Buffer;
class EventLoop;
class Connection;
class UnixAddress;
class Connector {
 public:
  using MessageCallback = std::function<bool (Connection *, Buffer &)>;
  using WriteableCallback = std::function<void()>;

  Connector(EventLoop *loop, const IPAddress &address);
  Connector(EventLoop *loop, const UnixAddress &address);
  ~Connector() = default;

  void StartNewConnection();
  void SendData(const char *data, size_t total);
  void SendFile(int file_fd, size_t total);
  void ShutdownConnection();

  // copy TCPServer callback -> TCPConnection, can't move, we'll create much TCPConnection by TCPServer::*callback
  void SetMessageCallback(const MessageCallback &callback);
  void SetWriteCallback(const WriteableCallback &callback);
  const MessageCallback &message_callback() const;
  const WriteableCallback &writeable_callback() const;

  int fd() const { return fd_; }
  auto connection_ptr() const -> std::shared_ptr<Connection>;
 private:
  void HandleEvent();

  int fd_;                                                                     // Not RAII Handle, only store the fd, resource managment to TCPConnection
  EventLoop *loop_;                                                            // use loop to control resource, like AddChannel()
  std::unique_ptr<Address> address_;                                           // Address, Bind with this
  std::shared_ptr<Connection> connection_ptr_;
  WriteableCallback write_callback_;
  MessageCallback message_callback_;
};

}

#endif //PLATINUM_CONNECTOR_H
