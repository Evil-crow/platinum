/**
 * Created by Crow on 11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe EPoller,
 *        Not like muduo::Poller/EPoller
 *        platinum::EPoller use epoll-style interface, not poll-style
 */


#ifndef PLATINUM_REACTOR_EPOLLER_H
#define PLATINUM_REACTOR_EPOLLER_H

#include <sys/epoll.h>
#include <memory>
#include <vector>

namespace platinum {

class Channel;
class EventLoop;
class EPoller {
 public:
  explicit EPoller(EventLoop *loop);
  ~EPoller();

  void EPoll(int timeout, std::vector<Channel*> &active_channel);
  void AddChannel(Channel *channel);
  void UpdateChannel(Channel *channel);
  void RemoveChannel(int fd);

 private:
  void FillActiveChannel(int eventnums, std::vector<Channel*> &active_channel);

  EventLoop *loop_;
  int epoll_fd_;
  std::vector<epoll_event> events_;
  static constexpr int MAXEPOLLEVENT() { return 1024; };
};

}

#endif //PLATINUM_REACTOR_EPOLLER_H
