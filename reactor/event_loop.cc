//
// Created by Crow on 11/25/18.
//

#include "event_loop.h"

#include <thread>
#include <utility/logger.h>

using namespace platinum;

thread_local EventLoop *t_event_loop = nullptr;

EventLoop::EventLoop()
    : looping_(false), thread_id_(std::this_thread::get_id())
{
  t_event_loop = this;
}

EventLoop::~EventLoop()
{
  if (!looping_)
    t_event_loop = nullptr;
}

void EventLoop::loop() {}

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