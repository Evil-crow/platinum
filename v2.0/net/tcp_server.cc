/**
 * Created by Crow on 12/11/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the Class TcpServer
 * @update 1/3/19   Add Connector to TcpServer
 */

#include "tcp_server.h"

#include <cassert>
#include <iostream>
#include "net/acceptor.h"
#include "net/connector.h"
#include "net/connection.h"
#include "reactor/event_loop.h"
#include "reactor/event_loop_pool.h"
#include "utility/logger.h"

using namespace platinum;

thread_local TcpServer *t_tcp_server = nullptr;

TcpServer::TcpServer(EventLoop *loop, IPAddress &address)
    : loop_(loop),
      acceptor_(std::make_unique<Acceptor>(loop, address)),
      pool_(std::make_unique<EventLoopPool>(loop)),
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
  pool_->Start();
  loop_->Loop();
}

void TcpServer::OnConnectionCallback(int fd, const IPAddress &peer_address)
{
  loop_->AssertInLoopThread();

  auto local_addr = socket::GetSockName(fd);
  IPAddress local_address(local_addr);

  std::string str(peer_address.ip() + ":" + std::to_string(peer_address.port()));
  LOG(INFO) << std::move(str);

  auto io_loop = pool_->GetNextLoop();
  auto connection_ptr = std::make_shared<Connection>(io_loop, fd, ParserType::HTTP);
  connection_ptr->SetMessageCallback(message_callback_);
  connection_ptr->SetConnectionCallback([this]() { connection_callback_(); });
  connection_ptr->SetCloseCallback([connection_ptr, this](){ EraseConnection(connection_ptr); });
  io_loop->RunInLoop([connection_ptr]() { connection_ptr->ConnectionEstablished(); });

  connections_.emplace(fd, connection_ptr);
}

void TcpServer::NewConnection(const Connector *connector)
{
  t_tcp_server->NewConnectionImpl(connector);
}

void TcpServer::NewConnectionImpl(const Connector *connector)
{
  auto fd = connector->fd();
  auto connection_ptr = connector->connection_ptr();
  connection_ptr->SetMessageCallback(connector->message_callback());
  connection_ptr->SetWriteCallback(connector->writeable_callback());
  connection_ptr->SetCloseCallback([=]() { EraseConnection(connection_ptr); });
  connection_ptr->ConnectionEstablished();

  connections_.emplace(fd, connection_ptr);
}

void TcpServer::EraseConnection(const std::shared_ptr<Connection> &connection_ptr)
{
  loop_->RunInLoop([connection_ptr, this](){ EraseConnectionInLoop(connection_ptr); });
}

void TcpServer::EraseConnectionInLoop(const std::shared_ptr<Connection> &connection_ptr)
{
  auto fd = connection_ptr->GetFd();
  auto io_loop = connection_ptr->GetLoop();

  loop_->AssertInLoopThread();
  auto t = connections_.erase(fd);
  assert(t == 1);
  io_loop->RunInLoop([connection_ptr]() { connection_ptr->ConnectionDesctroyed(); });
}

auto TcpServer::NewConnector(const IPAddress &address, ParserType type) -> std::shared_ptr<Connector>
{
  return t_tcp_server->NewConnectorImpl(address, type);
}

auto TcpServer::NewConnectorImpl(const IPAddress &address, ParserType type) -> std::shared_ptr<Connector>
{
  return std::make_shared<Connector>(loop_, address, type);
}

auto TcpServer::NewConnector(const UnixAddress &address, ParserType type) -> std::shared_ptr<Connector>
{
  return t_tcp_server->NewConnectorImpl(address, type);
}

auto TcpServer::NewConnectorImpl(const UnixAddress &address, ParserType type) -> std::shared_ptr<Connector>
{
  return std::make_shared<Connector>(loop_, address, type);
}

void TcpServer::SetConnectionCallback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void TcpServer::SetMessageCallback(const MessageCallback &callback)
{
  message_callback_ = callback;
}

void TcpServer::ForceClose(int fd)
{
  t_tcp_server->ForceClose(fd);
}

void TcpServer::ForceCloseImpl(int fd)
{
  fprintf(stderr, "use Force");
  if (connections_.find(fd) != connections_.end()) {
    connections_.erase(fd);
    LOG(INFO) << "TcpServer::ForceClose()";
  } else {
    LOG(ERROR) << "TcpServer::ForceClose() => Invalid fd to Close";
    std::abort();
  }
}

void TcpServer::SetThreadNum(int num)
{
  pool_->SetThreadNum(num);
}

