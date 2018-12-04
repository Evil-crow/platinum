//
// Created by Crow on 11/25/18.
//

#include "epoller.h"

#include <unistd.h>
#include <sys/epoll.h>
#include <utility/logger.h>

#include "reactor/event_loop.h"
#include "reactor/channel.h"

using namespace platinum;

EPoller::EPoller(platinum::EventLoop *loop)
    : loop_(loop),
      epoll_fd_(::epoll_create1(EPOLL_CLOEXEC))
{
  ;
}

// RAII Handle
EPoller::~EPoller()
{
  ::close(epoll_fd_);
}

void EPoller::Poll(int timeout, std::vector<std::shared_ptr<Channel>> &active_channels)
{
  loop_->AssertInLoopThread();
  auto event_nums = ::epoll_wait(epoll_fd_,
                                 event_list.data(),
                                 MAXEPOLLEVENT(),
                                 timeout);
  LOG(INFO) << "Poll Active Channels";
  FillActiveChannel(event_nums, active_channels);
}

void EPoller::FillActiveChannel(int event_nums, std::vector<std::shared_ptr<Channel>> &active_channel)
{
  Channel *channelptr(nullptr);
  for (auto var : event_list) {
    channelptr = new Channel(loop_, var.data.fd);                // Constructor Channel Object;
    channelptr->SetEvents(var.events);                           // Set the return event to Channel::revents;
    active_channel.push_back(channelptr);                        // push the Channel into vector to EventLoop;
  }
}

// We provide these two interface,
//
// EPoller::AddChannel()            => for ::epoll_ctl(EPOLL_CTL_ADD);
// EPoller::RemoveChannel()         => for ::epoll_ctl(EPOLL_CTL_DEL)
//
// We don't provide a interface for ::epoll_ctl(EPOLL_CTL_MOD),
// Because the degree of coupling of ::epoll_ctl(EPOLL_CTL_MOD) is high.
// We use Channel::Enable*() + Channel::Disable*() to replace this part function.

void EPoller::AddChannel(Channel *channel)
{
  loop_->AssertInLoopThread();

  epoll_event event{};
  event.data.fd = channel->fd();
  event.events = channel->events();
  if (::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel->fd(), &event) < 0) {
    LOG(ERROR) << "EPoller Add ERR";
    std::abort();
  }
}

void EPoller::RemoveChannel(platinum::Channel *channel)
{
  loop_->AssertInLoopThread();

  if (::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel->fd(), nullptr) < 0) {
    LOG(ERROR) << "EPoller Del ERR";
    std::abort();
  }
}
