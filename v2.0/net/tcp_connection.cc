//
// Created by Crow on 12/11/18.
//

#include "tcp_connection.h"

#include <iostream>
#include <unistd.h>
#include <sys/sendfile.h>

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
  channel_->EnableHangUp();
  channel_->EnableError();
  channel_->SetReadCallback  ([this]()  { TCPConnection::HandleRead(); });
  channel_->SetWriteCallback ([this]()  { TCPConnection::HandleWrite(); });
  channel_->SetCloseCallback ([this]()  { TCPConnection::HandleClose(); });
  channel_->SetErrorCallback ([this]()  { TCPConnection::HandleError(); });
  loop_->AddChannel(channel_.get());
}

void TCPConnection::ShutDownConnection()
{
  ShutdownInLoop();
}

void TCPConnection::ShutdownInLoop()
{
  loop_->RunInLoop([this]() { socket::ShutdownWrite(socket_->fd()); });
}

void TCPConnection::SendData(const char *data, size_t total) {
  size_t completed_(0), remained_(total);
  while (true) {
    auto var = ::write(socket_->fd(), data, total);
    if (var > 0) {
      if (var == total) {
        return ;
      } else {
        completed_ += var;
        remained_ -= var;
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        channel_->EnableWriteing();
        loop_->UpdateChannel(channel_.get());
        write_queue_.TaskInQueue(socket_->fd(), data, completed_, remained_);
      } else {
        LOG(ERROR) << "TCPConnection::SendData()";
      }
    }
  }
}

void TCPConnection::SendFile(int file_fd, size_t total)
{
  size_t completed_(0), remained_(total);
  while (true) {
    auto var = ::sendfile64(socket_->fd(), file_fd, nullptr, total);
    if (var > 0) {
      if (var == total) {
        return ;
      } else {
        completed_ += var;
        remained_ -= var;
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        channel_->EnableWriteing();
        loop_->UpdateChannel(channel_.get());
        write_queue_.TaskInQueue(socket_->fd(), file_fd, completed_, remained_);
      } else {
        LOG(ERROR) << "TCPConnection::SendData()";
      }
    }
  }
}

void TCPConnection::HandleRead() {
  read_buffer_.ReadFd(socket_->fd());
  if (message_callback_(this, read_buffer_))
    read_buffer_.Reprepare();
}

void TCPConnection::HandleWrite()
{
  if (write_queue_.DoTask()) {
    channel_->DisableWriting();
    loop_->UpdateChannel(channel_.get());
  }
}

void TCPConnection::HandleClose()
{
  loop_->RunInLoop([this]() { close_callback_(socket_->fd()); });
}

void TCPConnection::HandleError()
{
  loop_->RunInLoop([this]() { ErrorCallback(); });
}

void TCPConnection::ErrorCallback()
{
  LOG(ERROR) << "TCPConnection::ErrorCallback()";
  HandleClose();
}

void TCPConnection::SetConnectionCallback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void TCPConnection::SetMessageCallback(const MessageCallback &callback)
{
  message_callback_ = callback;
}

void TCPConnection::SetCloseCallback(const CloseCallback &callback)
{
  close_callback_ = callback;
}

