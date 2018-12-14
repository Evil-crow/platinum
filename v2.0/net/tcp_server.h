//
// Created by Crow on 12/11/18.
//

#ifndef PLATINUM_TCP_SERVER_H
#define PLATINUM_TCP_SERVER_H

#include <memory>
#include <atomic>
#include <functional>
#include <unordered_map>

#include "net/acceptor.h"

namespace platinum {

class Socket;
class IPAddress;
class EventLoop;
class TCPConnection;
class TCPServer {
 public:
  using EventCallback = std::function<void ()>;

  TCPServer(EventLoop *loop, IPAddress &address);
  ~TCPServer() = default;

  void Start();                                                             // When set all thing, we can start the server;
  void SetConnectionCallback(const EventCallback &callback);                // I don't use it usually, It means resource initlization for users connection
  void SetMessageCallback(const EventCallback &callback);                   // When message on, callback for user to perform logical business
                                              // Erase the TCPConnection which will be removed

 private:
  void OnConnectionCallback(int fd, const IPAddress &address);             // private method for Acceptor::callback_; to construct TCPConnection
  void EraseConnection(int fd);

  EventLoop *loop_;
  std::unique_ptr<Acceptor> acceptor_;                               // TCPServer holds Acceptor exclusivly
  std::unordered_map<int, std::shared_ptr<TCPConnection>> TCPConnectionMap_;              // use unordered_map to improve search efficiency
  EventCallback connection_callback_;
  EventCallback message_callback_;
  std::atomic<bool> starting_;
};

}

#endif //PLATINUM_TCP_SERVER_H
