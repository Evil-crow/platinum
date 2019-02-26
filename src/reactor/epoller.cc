/**
 * Created by Crow on 11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente EPoller,
 *        all interface will be called by loop_
 *        It means use EventLoop::[Add/Update/Remove]Channel() to operate
 */

#include "reactor/epoller.h"

#include <unistd.h>
#include <sys/epoll.h>

#include "utility/logger.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"

using namespace platinum;

EPoller::EPoller(platinum::EventLoop *loop)
    : loop_(loop),
      epoll_fd_(::epoll_create1(EPOLL_CLOEXEC))
{
  events_.reserve(1024);
}

// RAII Handle
EPoller::~EPoller()
{
  ::close(epoll_fd_);
}

void EPoller::EPoll(int timeout, std::vector<Channel*> &active_channels)
{
  loop_->AssertInLoopThread();
  events_.clear();
  auto event_nums = ::epoll_wait(epoll_fd_,
                                 events_.data(),
                                 MAXEPOLLEVENT(),
                                 timeout);
  if (event_nums < 0) {
    LOG(ERROR) << "EPoller::EPoll() => Epoll Error";
    std::abort();
  }
  FillActiveChannel(event_nums, active_channels);
}

void EPoller::FillActiveChannel(int event_nums, std::vector<Channel*> &active_channel)
{
  for (size_t i = 0; i < event_nums; ++i) {
    auto channel = reinterpret_cast<Channel *>(events_[i].data.ptr);
    channel->set_events(events_[i].events);                                // Set the return event to Channel::revents;
    active_channel.push_back(channel);                                    // push the Channel into vector to EventLoop;
  }
}

// We provide these two interface,
//
// EPoller::AddChannel()            => for ::epoll_ctl(EPOLL_CTL_ADD);
// EPoller::RemoveChannel()         => for ::epoll_ctl(EPOLL_CTL_DEL)
//
// We don't provide a interface for ::epoll_ctl(EPOLL_CTL_MOD),
// Because the degree of coupling of ::epoll_ctl(EPOLL_CTL_MOD) is high.
// We use Channel::Enable*() + Channel::Disable*() to replace this part of functions.

void EPoller::AddChannel(Channel *channel)
{
  loop_->AssertInLoopThread();

  epoll_event event{};
  event.data.ptr = channel;
  event.events = channel->events();
  if (::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel->fd(), &event) < 0) {
    LOG(ERROR) << "EPoller::AddChannel() => Add Channel Error";
    std::abort();
  }
}

void EPoller::UpdateChannel(Channel *channel)
{
    loop_->AssertInLoopThread();

    epoll_event event{};
    event.data.ptr = channel;
    event.events = channel->events();
    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, channel->fd(), &event) < 0) {
        LOG(ERROR) << "EPoller::UpdateChannel() => Update Channel Error";
        std::abort();
    }
}

void EPoller::RemoveChannel(int fd)
{
  loop_->AssertInLoopThread();

  if (::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
    LOG(ERROR) << "EPoller::RemoveChannel() => Remove Channel Error";
    perror("epoll_ctl");
    std::abort();
  }
}
