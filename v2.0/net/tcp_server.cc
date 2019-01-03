//
// Created by Crow on 12/11/18.
//

#include "tcp_server.h"

#include <iostream>
#include "net/acceptor.h"
#include "net/connector.h"
#include "net/connection.h"
#include "reactor/event_loop.h"
#include "utility/logger.h"

using namespace platinum;

thread_local TcpServer *t_tcp_server = nullptr;

TcpServer::TcpServer(EventLoop *loop, IPAddress &address)
    : loop_(loop),
      acceptor_(std::make_unique<Acceptor>(loop, address)),
      starting_(false)
{
  if (t_tcp_server) {
    LOG(ERROR) << "TcpServer::Tcpserver() => one server per thread";
    std::abort();
  } else {
    t_tcp_server = this;
    LOG(INFO) << "TcpServer has been created";
  }
}

void TcpServer::Start()
{
  starting_.store(true);
  acceptor_->SetConnectionCallback(
      [this](int fd, const IPAddress& addr) {
        OnConnectionCallback(fd, addr);
      });
  acceptor_->Listening();
  loop_->Loop();
}

void TcpServer::OnConnectionCallback(int fd, const IPAddress &peer_address)
{
  loop_->AssertInLoopThread();

  auto local_addr = socket::GetSockName(fd);
  IPAddress local_address(local_addr);

  std::string str(peer_address.ip() + ":" + std::to_string(peer_address.port()));
  LOG(INFO) << std::move(str);

  auto connection_ptr = std::make_shared<Connection>(loop_, fd);
  connection_ptr->SetMessageCallback(message_callback_);
  connection_ptr->SetConnectionCallback([this]() { connection_callback_(); });
  connection_ptr->SetCloseCallback([this](int fd){ EraseConnection(fd); });
  connection_ptr->ConnectionEstablished();

  connections_.emplace(fd, connection_ptr);
}

void TcpServer::NewConnection(const Connector *connector)
{
  auto fd = connector->fd();
  auto connection_ptr = connector->connection_ptr();
  connection_ptr->SetMessageCallback(connector->message_callback());
  connection_ptr->SetWriteCallback(connector->writeable_callback());
  connection_ptr->SetCloseCallback([this](int fd) { EraseConnection(fd); });
  connection_ptr->ConnectionEstablished();

  connections_.insert({fd, connection_ptr});
}

void TcpServer::EraseConnection(int fd) {
  loop_->AssertInLoopThread();
  if (connections_.find(fd) != connections_.end()) {
    loop_->RemoveChannel(fd);
    connections_.erase(fd);
  } else {
    LOG(INFO) << "TcpServer::EraseConnection()  => fd Invilid Error";
    std::abort();
  }
}

auto TcpServer::NewConnector(const IPAddress &address) -> std::shared_ptr<Connector>
{
  auto ptr = std::make_shared<Connector>(loop_, address);
  return ptr;
}

auto TcpServer::NewConnector(const UnixAddress &address) -> std::shared_ptr<Connector>
{
  auto ptr = std::make_shared<Connector>(loop_, address);
  return ptr;
}

void TcpServer::SetConnectionCallback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void TcpServer::SetMessageCallback(const MessageCallback &callback)
{
  message_callback_ = callback;
}

TcpServer *platinum::GetCurrentServer()
{
  return t_tcp_server;
}

void TcpServer::ForceClose(int fd)
{
  if (connections_.find(fd) != connections_.end()) {
    connections_.erase(fd);
    LOG(INFO) << "TcpServer::ForceClose() => Force Close Connection";
  } else {
    LOG(ERROR) << "TcpServer::ForceClose() => Not This Connection";
    std::abort();
  }
}
