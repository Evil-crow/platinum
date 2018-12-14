//
// Created by Crow on 12/11/18.
//

#include "tcp_server.h"

#include <iostream>
#include "net/acceptor.h"
#include "net/tcp_connection.h"
#include "reactor/event_loop.h"
#include "utility/logger.h"

using namespace platinum;

TCPServer::TCPServer(EventLoop *loop, IPAddress &address)
    : loop_(loop),
      acceptor_(std::make_unique<Acceptor>(loop, address)),
      starting_(false)
{
  ;
}

void TCPServer::Start()
{
  starting_.store(true);
  acceptor_->SetConnectionCallback(
      [this](int fd, const IPAddress& addr) {
        OnConnectionCallback(fd, addr);
      });
  acceptor_->Listening();
  loop_->Loop();
}

void TCPServer::OnConnectionCallback(int fd, const IPAddress &peer_address)
{
  loop_->AssertInLoopThread();
//  auto fd = socket.fd();                    // we'll move socket(Class Socket), so save the socket.fd().

  //pay attention to efficiency, use Container::insert() than Container::operator[]
  auto local_addr = socket::GetSockName(fd);
  IPAddress local_address(local_addr);

  std::cout << "fd: " << fd << std::endl;
  std::cout << "ip: " << peer_address.ip() << std::endl;
  std::cout << "port: " << peer_address.port() << std::endl;

  auto connection_ptr = std::make_shared<TCPConnection>(
      loop_, fd, peer_address, local_address
  );
  connection_ptr->SetMessageCallback([this]() { message_callback_(); });
  connection_ptr->SetConnectionCallback([this]() { connection_callback_(); });
  connection_ptr->SetCloseCallback([this](int fd){ EraseConnection(fd); });
  connection_ptr->ConnectionEstablished();

  TCPConnectionMap_.emplace(fd, connection_ptr);
  LOG(INFO) << "TCPServer::OnConnectionCallback Connection set OK!";
}

void TCPServer::EraseConnection(int fd)
{
  loop_->AssertInLoopThread();
  if (TCPConnectionMap_.find(fd) != TCPConnectionMap_.end()) {
    loop_->RemoveChannel(fd);
    TCPConnectionMap_.erase(fd);
  }
  LOG(INFO) << "Erase TCPConnection from TCPServerMap";
}

void TCPServer::SetConnectionCallback(const EventCallback &callback)
{
  connection_callback_ = callback;
}

void TCPServer::SetMessageCallback(const EventCallback &callback)
{
  message_callback_ = callback;
}