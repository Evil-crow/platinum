//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_EPOLLER_H
#define PLATINUM_EPOLLER_H

#include <vector>
#include <sys/epoll.h>

namespace platinum {

class Channel;
class EventLoop;
class EPoller {
 public:
  explicit EPoller(EventLoop *loop);
  ~EPoller();

  void Poll(int timeout, std::vector<Channel *> &active_channel);
  void AddChannel(Channel *channel);
  void RemoveChannel(Channel *channel);

 private:
  void FillActiveChannel(int eventnums, std::vector<Channel *> &active_channel);

  EventLoop *loop_;
  int epoll_fd_;
  std::vector<epoll_event> event_list;
  static constexpr int MAXEPOLLEVENT() { return 1024; };
};

}

#endif //PLATINUM_EPOLLER_H
