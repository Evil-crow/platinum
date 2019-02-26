/**
 * Created by Crow on 12/11/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the most important class Connection
 *        This Class had been named TcpConnection
 *        But, for FCGI, WS must be play a role as Client
 *        So, convert TcpConnection to Connection
 */

#include "connection.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include "net/socket.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"
#include "protocol/http/request_parser.h"
#include "protocol/fastCGI/response_parser.h"
#include "utility/logger.h"

using namespace platinum;

Connection::Connection(EventLoop *loop, int fd, ParserType type)
    : loop_(loop),
      socket_(std::make_unique<Socket>(fd)),           // use move constructor
      channel_(std::make_unique<Channel>(loop, fd)),
      parser_(std::make_unique<http::RequestParser>()) {
  switch (type) {
    case ParserType::FCGI:
      parser_ = std::make_unique<fcgi::ResponseParser>(); break;
    case ParserType::HTTP:
      parser_ = std::make_unique<http::RequestParser>(); break;
    default:
      parser_ = std::make_unique<http::RequestParser>(); break;
  }
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
  channel_->set_read_callback  ([this]() { HandleRead(); });
  channel_->set_write_callback ([this]() { HandleWrite(); });
  channel_->set_close_callback ([this]() { HandleClose(); });
  channel_->set_error_callback ([this]() { HandleError(); });
  loop_->AddChannel(channel_.get());
}

void Connection::ConnectionDesctroyed()
{
  loop_->AssertInLoopThread();
  channel_->DisableALL();
  loop_->RemoveChannel(socket_->fd());
}

void Connection::ShutDownConnection()
{
  ShutdownInLoop();
}

void Connection::ShutdownInLoop()
{
  loop_->RunInLoop([this]() { socket::ShutdownWrite(socket_->fd()); });
}

void Connection::ForceClose()
{
  ForceCloseInLoop();
}

void Connection::ForceCloseInLoop()
{
  loop_->RunInLoop([this]() { close_callback_(socket_->fd()); });
}

void Connection::SendData(const unsigned char *data, size_t total)
{
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
        LOG(ERROR) << "Connection::SendData() => Send Data Error";
        std::abort();
      }
    }
  }
}

void Connection::SendFile(const std::string &pathname, size_t total)
{
  size_t completed_(0), remained_(total);

  int file_fd = ::open(pathname.c_str(), O_RDONLY, 0644);
  if (file_fd < 0) {
    LOG(ERROR) << "Connection::SendFile() => Open File Error";
    std::abort();
  }

  while (true) {
    auto var = ::sendfile64(socket_->fd(), file_fd, nullptr, total);
    if (var > 0) {
      if (var == total) {                                                      // Close file after write OK, needn't empalce in write queue
        if (::close(file_fd) < 0) {
          LOG(ERROR) << "Connection::SendFile() => Close File Error";
          std::abort();
        }
        return ;
      } else {
        completed_ += var;
        remained_ -= var;
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        channel_->EnableWriteing();
        loop_->UpdateChannel(channel_.get());
        write_queue_.TaskInQueue(socket_->fd(), pathname, completed_, remained_);
      } else {
        LOG(ERROR) << "Connection::SendFile() => Send File Error";
      }
    }
  }
}

void Connection::HandleRead() {
  read_buffer_.ReadFd(socket_->fd());
  auto len = message_callback_(this, read_buffer_, parser_);
  read_buffer_.Reprepare(len);
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
  LOG(ERROR) << "Connection::ErrorCallback() => Error Occured";
  HandleClose();
}

void Connection::set_connection_callback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void Connection::set_message_callback(const MessageCallback &callback)
{
  message_callback_ = callback;
}

void Connection::set_close_callback(const platinum::Connection::CloseCallback &callback)
{
  close_callback_ = callback;
}

void Connection::set_write_callback(const Connection::EventCallback &callback)
{
  write_callback_ = callback;
}

void Connection::set_forward_fd(int fd)
{
  forward_fd_ = fd;
}

int Connection::forward_fd()
{
  return forward_fd_;
}

int Connection::socket_fd()
{
  return socket_->fd();
}
