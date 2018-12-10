//
// Created by Crow on 12/10/18.
//

#include "net/acceptor.h"

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
      listenfd_(socket::Socket()),
      channel_(std::make_unique<Channel>(loop, listenfd_.fd())),
      listening_(false)
{
  listenfd_.SetReusePort(true);
  listenfd_.SetTcpNoDelay(true);
  listenfd_.SetKeepAlive(true);
}

void Acceptor::Listening()
{
  listening_.store(true);
  listenfd_.Bind(address_);
  listenfd_.Listen();
  channel_->EnableET();
  channel_->EnableReading();
  channel_->SetReadCallback([this]() { HandleEvent(); });
  loop_->AddChannel(channel_.get());
}

void Acceptor::HandleEvent()
{
  IPAddress peer_address;
  while (true) {
    int connfd = listenfd_.Accept(peer_address);
    if (connfd > 0) {
      if (callback_)
        callback_(connfd, peer_address);
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG(INFO) << "Acceptor::HandleEvent Accept OK!";
        break;
      } else {
        LOG(ERROR) << "Acceptor::HAndleEvent ERR";
        std::abort();
      }
    }
  }
}

bool Acceptor::IsListening()
{
  return listening_.load();
}

void Acceptor::SetConnectionCallback(Acceptor::NewConnectionCallback callback)
{
  callback_ = std::move(callback);
}


