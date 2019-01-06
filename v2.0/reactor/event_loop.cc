/**
 * Created by Crow on 11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente EventLoop,
 *        The core to immplmente Non-block IO Server,
 *        To use Event-driven style server by EventLoop
 */

#include "reactor/event_loop.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <thread>

#include "utility/logger.h"
#include "net/tcp_server.h"
#include "reactor/epoller.h"
#include "reactor/channel.h"

using namespace platinum;

thread_local EventLoop *t_event_loop = nullptr;            // thread_local, to indicate there is only one eventloop in the thread.

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      eventing_handling_(false),
      wakeup_fd_(::eventfd(0, 0)),
      epoller_(new EPoller(this)),
      wakeup_channel_(new Channel(this, wakeup_fd_)),
      thread_id_(std::this_thread::get_id())
{
  if (t_event_loop) {
    LOG(ERROR) << "EventLoop::EventLoop() => One Loop per thread";
    AbortInLoopThread();
  } else {
    t_event_loop = this;
    LOG(INFO) << "EventLoop has been created";
  }

  wakeup_channel_->EnableReading();
  wakeup_channel_->SetReadCallback([this]() { HandleWakeUp(); });
  epoller_->AddChannel(wakeup_channel_.get());
}

EventLoop::~EventLoop()
{
  if (!looping_.load())
    t_event_loop = nullptr;
}

void EventLoop::Loop()
{
  looping_.store(true);
  quit_.store(false);

  AssertInLoopThread();
  while (!quit_.load()) {
    eventing_handling_.store(true);
    active_channels_.clear();
    epoller_->EPoll(-1, active_channels_);
    for (const auto &channel : active_channels_)
      channel->HandleEvent();
    DoPendingFunctors();
    eventing_handling_.store(false);
  }

  looping_.store(false);
}

void EventLoop::Quit()
{
  quit_.store(false);
  if (!eventing_handling_.load())
    WakeUp();
}

void EventLoop::AddChannel(Channel *channel)
{
  if (!IsInLoopThread()) {
    LOG(ERROR) << "EventLoop::AddChannel() => Not In This Loop";
    std::abort();
  }
  epoller_->AddChannel(channel);
}

void EventLoop::UpdateChannel(Channel *channel)
{
    if (!IsInLoopThread()) {
        LOG(ERROR) << "EventLoop::UpdateChannel() => Not In This Loop";
        std::abort();
    }
    epoller_->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(int fd)
{
  if (!IsInLoopThread()) {
    LOG(ERROR) << "EventLoop::RemoveChannel() => Not In This Loop";
    std::abort();
  }
  epoller_->RemoveChannel(fd);
}

void EventLoop::WakeUp()
{
  uint64_t u;
  ::write(wakeup_fd_, &u, sizeof(u));
  LOG(INFO) << "Wake Up Event";
}

void EventLoop::HandleWakeUp()
{
  uint64_t u;
  ::read(wakeup_fd_, &u, sizeof(u));
  LOG(INFO) << "Handle Wake Event";
}

bool EventLoop::AssertInLoopThread()
{
  if (!IsInLoopThread())
    AbortInLoopThread();
  return true;
}

bool EventLoop::IsInLoopThread()
{
  return thread_id_ == std::this_thread::get_id();
}

void EventLoop::AbortInLoopThread()
{
  LOG(ERROR) << "EventLoop::AbortInLoopThread() => Abort ";
  std::abort();
}

void EventLoop::DoPendingFunctors()
{
  calling_pending_functor_.store(true);
  std::vector<Functor> temp;               // Minimize the critical zone size as much as possible
  {
    std::lock_guard<std::mutex> lk(mutex_);
    temp.swap(pending_functors_);
  }
  for (const auto &functor : temp)        // Don't use while(vec.func.empty()), because you may stay here forever, when task is heavy
    functor();
  calling_pending_functor_.store(false);
}

void EventLoop::RunInLoop(const EventLoop::Functor &cb)
{
    QueueInLoop(cb);
}

void EventLoop::QueueInLoop(const EventLoop::Functor &cb)
{
  {
    std::lock_guard<std::mutex> lk(mutex_);
    pending_functors_.push_back(cb);
  }
  if (!IsInLoopThread() || calling_pending_functor_.load())
    WakeUp();
}
