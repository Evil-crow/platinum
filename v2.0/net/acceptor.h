/**
 * Created by Crow on 11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This class will be used to control listen socket
 *        like TCPConnection to control the connect fd
 *
 *    Tips: At first time, I prepare to use getpeername(2) to get the peer address
 *    But muduo use
 *            int accept(IPAddress &addr)
 *    to get the address, yeah, It can save a time syscall,
 *    Emmm, It can be so useful.
 *    This class test file in /test/acceptor_test.cc
*/

#ifndef PLATINUM_NET_ACCEPTOR_H
#define PLATINUM_NET_ACCEPTOR_H

#include <atomic>
#include <memory>
#include <functional>

#include "reactor/channel.h"
#include "net/socket.h"
namespace platinum {

class Socket;                     // forward declaration
class Channel;                    // forward declaration
class IPAddress;                  // forward declaration
class EventLoop;                  // forward declaration

class Acceptor {
 public:
  using NewConnectionCallback = std::function<void(int, const IPAddress &)>;         // for connection callback
  Acceptor(EventLoop *loop, const IPAddress &address);
  ~Acceptor() = default;

  void Listening();                                                            // start listening
  void HandleEvent();                                                          // deal with the readable event -> Get Connection
  void SetConnectionCallback(const NewConnectionCallback &callback);           // To Set callback event when on connection

 private:
  bool IsListening();

  EventLoop *loop_;                                                            // use loop to control resource, like AddChannel()
  Socket listenfd_;                                                            // RAII Handle, Listen Socket, the same as connfd_ to TCPConnection
  IPAddress address_;                                                          // IPAddress, Bind with this
  std::unique_ptr<Channel> channel_;                                           // std::unique_ptr to manage channel
  NewConnectionCallback callback_;
  std::atomic<bool> listening_;
};

}

#endif //PLATINUM_NET_ACCEPTOR_H
