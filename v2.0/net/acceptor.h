//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_ACCEPTOR_H
#define PLATINUM_ACCEPTOR_H

#include <atomic>
#include <memory>
#include <functional>

#include "reactor/channel.h"
#include "net/socket.h"
namespace platinum {

class Channel;                    // forward declaration
class IPAddress;                  // forward declaration
class EventLoop;                  // forward declaration

class Acceptor {
 public:
  using NewConnectionCallback = std::function<void(int, const IPAddress &)>;         // for connection callback
  explicit Acceptor(EventLoop *loop, const IPAddress &address);
  ~Acceptor() = default;

  void Listening();                                                            // start listening
  void HandleEvent();                                                          // deal with the readable event -> Get Connection
  void SetConnectionCallback(NewConnectionCallback callback);

 private:
  bool IsListening();

  EventLoop *loop_;
  Socket listenfd_;
  IPAddress address_;
  std::unique_ptr<Channel> channel_;
  NewConnectionCallback callback_;
  std::atomic<bool> listening_;
};

}

#endif //PLATINUM_ACCEPTOR_H
