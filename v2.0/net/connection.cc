//
// Created by Crow on 12/11/18.
//

#include "connection.h"

#include <iostream>
#include <unistd.h>
#include <sys/sendfile.h>

#include "net/socket.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"
#include "utility/logger.h"

using namespace platinum;

Connection::Connection(EventLoop *loop, int fd)
    : loop_(loop),
      socket_(std::make_unique<Socket>(fd)),           // use move constructor
      channel_(std::make_unique<Channel>(loop, fd))
{
  ;
}

Connection::~Connection()
{
  ;
}

void Connection::ConnectionEstablished()
{
  channel_->EnableET();
  channel_->EnableWriteing();
  channel_->EnableReading();
  channel_->EnableHangUp();
  channel_->EnableError();
  channel_->SetReadCallback  ([this]()  { Connection::HandleRead(); });
  channel_->SetWriteCallback ([this]()  { Connection::HandleWrite(); });
  channel_->SetCloseCallback ([this]()  { Connection::HandleClose(); });
  channel_->SetErrorCallback ([this]()  { Connection::HandleError(); });
  loop_->AddChannel(channel_.get());
}

void Connection::ShutDownConnection()
{
  ShutdownInLoop();
}

void Connection::ShutdownInLoop()
{
  loop_->RunInLoop([this]() { socket::ShutdownWrite(socket_->fd()); });
}

void Connection::SendData(const char *data, size_t total) {
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
        LOG(ERROR) << "Connection::SendData()";
      }
    }
  }
}

void Connection::SendFile(int file_fd, size_t total)
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
        LOG(ERROR) << "Connection::SendData()";
      }
    }
  }
}

void Connection::HandleRead() {
  read_buffer_.ReadFd(socket_->fd());
  if (message_callback_(this, read_buffer_))
    read_buffer_.Reprepare();
}

void Connection::HandleWrite()
{
  if (write_callback_) {
    write_callback_();
  }
  if (write_queue_.DoTask()) {
    channel_->DisableWriting();
    loop_->UpdateChannel(channel_.get());
  }
}

void Connection::HandleClose()
{
  loop_->RunInLoop([this]() { close_callback_(socket_->fd()); });
}

void Connection::HandleError()
{
  loop_->RunInLoop([this]() { ErrorCallback(); });
}

void Connection::ErrorCallback()
{
  LOG(ERROR) << "Connection::ErrorCallback()";
  HandleClose();
}

void Connection::SetConnectionCallback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void Connection::SetMessageCallback(const MessageCallback &callback)
{
  message_callback_ = callback;
}

void Connection::SetCloseCallback(const CloseCallback &callback)
{
  close_callback_ = callback;
}

void Connection::SetWriteCallback(const Connection::EventCallback &callback)
{
  write_callback_ = callback;
}
