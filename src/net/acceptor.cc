/**
 * Created by Crow on 12/10/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe Class Acceptor
 */


#include "net/acceptor.h"

#include <cerrno>
#include <memory>

#include "net/socket.h"
#include "net/ip_address.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"
#include "utility/logger.h"

using namespace platinum;

Acceptor::Acceptor(EventLoop *loop, const IPAddress &address)
    : loop_(loop),
      address_(address),
      listenfd_(socket::Socket(socket::Type::INET)),
      channel_(std::make_unique<Channel>(loop, listenfd_.fd())),
      listening_(false)
{
  listenfd_.SetReusePort(true);                                   // REUSEPORT is more useful than REUSEADDR. when no condiser broadcast
  listenfd_.SetTcpNoDelay(true);                                  // No TCP Nagel
  listenfd_.SetKeepAlive(true);
}

void Acceptor::Listening()
{
  listening_.store(true);
  listenfd_.Bind(address_);
  listenfd_.Listen();

  // Set Channel insterested events
  channel_->EnableET();
  channel_->EnableReading();
  channel_->set_read_callback([this]() { Acceptor::HandleEvent(); });
  loop_->AddChannel(channel_.get());                           // Add Channel to EPoller
}

void Acceptor::HandleEvent()
{
  IPAddress peer_address;
  // Beacuse we use EPoll::ET + Non-Block, so when can't break until EAGAIN/EWOULDBLOCK
  while (true) {
    int connfd = listenfd_.Accept(peer_address);
    if (connfd > 0) {
      // WARNING: NOT copy ctor, RAII Handle will close fd, you have three methods:
      // 1. move arguments, NOT USE MOVE , It'll close Handle!
      // 2. use file descriptor, NOT directly RAII Handle,(RAII Handle will close the fd)
      // 3. use smart pointer. std::unique_ptr / std::shared_ptr.
      if (callback_)
        callback_(connfd, peer_address);
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG(INFO) << "Acceptor::HandleEvent";
        break;
      } else {
        LOG(ERROR) << "Acceptor::HAndleEvent() => Accept Error";
        std::abort();
      }
    }
  }
}

bool Acceptor::IsListening()
{
  return listening_.load();
}

void Acceptor::set_connection_callback(const NewConnectionCallback &callback)
{
  if (!IsListening())
    callback_ = callback;
}


