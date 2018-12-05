//
// Created by Crow on 11/25/18.
//

#include "channel.h"

#include <sys/epoll.h>

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
  ;
}

// We only distinguish readable and the opposite hang up
// the normal close and readable all will trigger EPOLLIN
// we distinguish these two situation, In TCPConnection::ReadFd()
void Channel::HandleEvent()
{
  event_handling_.store(true);
  if (revents_ & (EPOLLHUP | EPOLLIN)) {
    if (close_callback_)
      close_callback_();
  }
  if (revents_ & EPOLLIN) {
    if (read_callback_)
      read_callback_();
  }
  if (revents_ & EPOLLOUT) {
    if (write_callback_)
      write_callback_();
  }
  event_handling_.store(false);
}

void Channel::SetEvents(unsigned int events) { revents_ = events; }

void Channel::EnableReading()       { events_ |= EPOLLIN; }

void Channel::EnableWriteing()      { events_ |= EPOLLOUT; }

void Channel::EnableET()            { events_ |= EPOLLET; }

void Channel::DisableWriting()      { events_ &= ~EPOLLOUT; }

void Channel::DisableALL()          { events_ = 0; }
