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
  channel_->SetReadCallback([this]() { HandleEvent(); });
  loop_->AddChannel(channel_.get());                           // Add Channel to EPoller
}

void Acceptor::HandleEvent()
{
  IPAddress peer_address;
  while (true) {                                               // Beacuse we use EPoll::ET + Non-Block, so when can't break until EAGAIN/EWOULDBLOCK
    int connfd = listenfd_.Accept(peer_address);
    if (connfd > 0) {
      if (callback_)
        callback_(std::move(Socket(connfd)), std::move(peer_address));
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

void Acceptor::SetConnectionCallback(NewConnectionCallback callback)
{
  if (!IsListening())
    callback_ = std::move(callback);
}


