// Created by Crow on 11/25/18.
//

#include "event_loop.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <thread>
#include <utility/logger.h>

using namespace platinum;

thread_local EventLoop *t_event_loop = nullptr;            // thread_local, to indicate there is only one eventloop in the thread.

EventLoop::EventLoop()
    : looping_(false),
      wakeup_fd_(::eventfd(0, 0)),
      wakeup_channel_(new Channel(this, wakeup_fd_)),
      thread_id_(std::this_thread::get_id())
{
  if (t_event_loop) {
    LOG(ERROR) << "EventLoop has been created";
    AbortInLoopThread();
  } else {
    t_event_loop = this;
  }
}

EventLoop::~EventLoop()
{
  if (!looping_.load())
    t_event_loop = nullptr;
}

void EventLoop::Loop()
{
  while (!quit_.load()) {

  }
}

void EventLoop::Quit()
{
  quit_.store(false);
  if (!looping_.load())
    WakeUp();
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
  LOG(ERROR) << "EventLoop::AbortInLoopThread";
  std::abort();
}