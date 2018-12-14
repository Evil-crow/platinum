//
// Created by Crow on 12/11/18.
//

#include <iostream>
#include "tcp_connection.h"

#include "net/socket.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"
#include "utility/logger.h"

using namespace platinum;

TCPConnection::TCPConnection(EventLoop *loop,
                             int fd,
                             const IPAddress &local_address,
                             const IPAddress &peer_address)
    : loop_(loop),
      socket_(std::make_unique<Socket>(fd)),           // use move constructor
      channel_(std::make_unique<Channel>(loop, fd)),
      local_addrss_(local_address),
      peer_address_(peer_address)
{
  ;
}

TCPConnection::~TCPConnection()
{
  ;
}

void TCPConnection::ConnectionEstablished()
{
  channel_->EnableET();
  channel_->EnableReading();
  channel_->SetReadCallback  ([this]()  { TCPConnection::HandleRead(); });
  channel_->SetCloseCallBack ([this]()  { TCPConnection::HandleClose(); });
  loop_->AddChannel(channel_.get());
}

void TCPConnection::HandleRead()
{
  std::cout << "get into HandleRead()" << '\n';
  char buf[1000];
  auto ret = ::recv(socket_->fd(), buf, 1000, 0);
  std::cout << buf << std::endl;
  if (ret > 0) {
    std::cout << "ret > 0" << '\n';
    message_callback_();
  } else if (ret == 0) {
    HandleClose();
  } else {
    HandleError();
  }
}

void TCPConnection::HandleError()
{
  ErrorCallback();
}

void TCPConnection::HandleClose()
{
  if (close_callback_)
    close_callback_(socket_->fd());
}

void TCPConnection::ErrorCallback()
{
  LOG(WARN) << "TCPConnection::ErrorCallback: Handle Event Error, Close Connection";
  close_callback_(socket_->fd());
}

void TCPConnection::SetConnectionCallback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void TCPConnection::SetMessageCallback(const EventCallback &callback)
{
  message_callback_ = callback;
}

void TCPConnection::SetCloseCallback(const EventCallback &callback)
{
  close_callback_ = callback;
}

