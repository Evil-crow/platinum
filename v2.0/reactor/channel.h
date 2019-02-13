/**
 * Created by Crow on 11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the Class Channel
 */

#ifndef PLATINUM_REACTOR_CHANNEL_H
#define PLATINUM_REACTOR_CHANNEL_H

#include <atomic>
#include <functional>

namespace platinum {

class EventLoop;
class Channel {
 public:
  using EventCallback = std::function<void ()>;

  Channel(EventLoop *loop, int fd);
  ~Channel();

  void SetReadCallback(const EventCallback &cb)      { read_callback_ = cb; }
  void SetWriteCallback(const EventCallback &cb)     { write_callback_ = cb; }
  void SetCloseCallback(const EventCallback &cb)     { close_callback_ = cb; }
  void SetErrorCallback(const EventCallback &cb)     { error_callback_ = cb; }

  void HandleEvent();
  void SetEvents(unsigned int events);

  void EnableReading();
  void EnableWriteing();
  void EnableHangUp();
  void EnableError();
  void DisableWriting();
  void DisableALL();
  void EnableET();

  int fd()                 { return fd_; }
  unsigned int events()    { return events_; }

 private:
  EventLoop *loop_;                                     // loop:   Eventloop, we need this to call some callback operation

  int fd_;                                              // fd:    to store the fd, but the RAII Handle is in EventLoop.
  unsigned int events_;                                 // event: to record which events we're instered in , for epoller.
  unsigned int revents_;                                // event: which events has happen on this Channel, fill by epoller
  std::atomic<bool> event_handling_;                    // bool:  to identify is handling event, can't destructor at now.

  // CallBacks, we use these callback to complete event driven model
  EventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;
};

}

#endif //PLATINUM_REACTOR_CHANNEL_H
