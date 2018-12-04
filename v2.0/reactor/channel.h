//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_TCP_CHANNEL_H
#define PLATINUM_TCP_CHANNEL_H

#include <atomic>
#include <functional>

namespace platinum {

class EventLoop;
class Channel {
 public:
  using EventCallBack = std::function<void ()>;

  Channel(EventLoop *loop, int fd);
  ~Channel();

  void SetReadCallback(EventCallBack &&cb)      { read_callback_ = cb; }
  void SetWriteCallBack(EventCallBack &&cb)     { write_callback_ = cb; }
  void SetCloseCallBack(EventCallBack &&cb)     { close_callback_ = cb; }
  void SetErrorCallBack(EventCallBack &&cb)     { error_callback_ = cb; }

  void HandleEvent();
  void SetEvents(unsigned int events);
  void EnableReading();
  void EnableWriteing();
  void DisableWriting();
  void DisableALL();

  // Actually use EventLoop::*Channel() => Epoller::*Channel() to complete the operation
//  void AddChannel(Channel *channel_);
//  void RemoveChannel(Channel *channel_);

  int fd()                 { return fd_; }
  unsigned int events()    { return events_; }

 private:
  EventLoop *loop_;                                     // loop:   Eventloop, we need this to call some callback operation

  std::atomic<bool> event_handling_;                    // bool:  to identify is handling event, can't destructor at now.
  int fd_;                                              // fd:    to store the fd, but the RAII Handle is in EventLoop.
  unsigned int events_;                                 // event: to record which events we're instered in , for epoller.
  unsigned int revents_;                                // event: which events has happen on this Channel, fill by epoller

  // CallBacks, we use these callback to complete event driven model
  EventCallBack read_callback_;
  EventCallBack write_callback_;
  EventCallBack close_callback_;
  EventCallBack error_callback_;
};

}

#endif //PLATINUM_TCP_CHANNEL_H
