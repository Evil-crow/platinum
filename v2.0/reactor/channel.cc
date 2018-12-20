//
// Created by Crow on 11/25/18.
//

#include "reactor/channel.h"

#include <sys/epoll.h>
#include <cassert>

#include "reactor/event_loop.h"

using namespace platinum;

Channel::Channel(platinum::EventLoop *loop, int fd)
  : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    event_handling_(false)
{
  ;
}

Channel::~Channel()
{
  assert(!event_handling_.load());                  // To make sure we won't destruct Channel when event handling;
}


// We only distinguish readable and the opposite hang up
// the normal close and readable all will trigger EPOLLIN
// we distinguish these two situation, In TCPConnection::ReadFd()
void Channel::HandleEvent()
{
  event_handling_.store(true);

  // handle error
  if (revents_ & EPOLLERR) {
    if (error_callback_)
      error_callback_();
  }
  // handle hangup event
  else if (revents_ & EPOLLRDHUP) {                  // EPOLLRDHUP => EPOLLIN | EPOLLHUP
    if (close_callback_)
      close_callback_();
  }
  // handle readable event, e.g. OnMessage
  else if (revents_ & EPOLLIN) {
    if (read_callback_)
      read_callback_();
  }
  // handle writeable event, e.g. WriteQueue
  else if (revents_ & EPOLLOUT) {
    if (write_callback_)
      write_callback_();
  }
  event_handling_.store(false);
}

void Channel::SetEvents(unsigned int events) { revents_ = events; }

void Channel::EnableReading()       { events_ |= EPOLLIN; }

void Channel::EnableWriteing()      { events_ |= EPOLLOUT; }

void Channel::EnableET()            { events_ |= EPOLLET; }

void Channel::EnableHangUp()        { events_ |= EPOLLRDHUP; }

void Channel::EnableError()         { events_ |= EPOLLERR; }

void Channel::DisableWriting()      { events_ &= ~EPOLLOUT; }

void Channel::DisableALL()          { events_ = 0; }
