//
// Created by Crow on 12/11/18.
//

#include "tcp_server.h"

#include <memory>

#include "net/acceptor.h"
#include "reactor/event_loop.h"
#include "utility/logger.h"

using namespace platinum;

TCPServer::TCPServer(EventLoop *loop, IPAddress &address)
    : loop_(loop),
      acceptor_(std::make_unique<Acceptor>(loop, address)),
      starting_(false)
{

}

void TCPServer::Start()
{
  starting_.store(true);
  acceptor_->SetConnectionCallback(OnConnectionCallback);
  acceptor_->Listening();
  loop_->Loop();
}

void TCPServer::OnConnectionCallback(Socket &&socket, IPAddress &&address)
{
  loop_->AssertInLoopThread();

  //pay attention to efficiency, use Container::insert() than Container::operator[]
  auto local_addr = socket::GetSockName(socket.fd());
  IPAddress local_address(local_addr);

  TCPConnectionMap_.insert({
      socket.fd(), std::make_shared(loop_, std::move(socket), address, local_address)
  });
  LOG(INFO) << "TCPServer::OnConnectionCallback Connection set OK!";
}

void TCPServer::SetConnectionCallback(const TCPServer::EventCallback &callback)
{
  connection_callback_ = callback;
}

void TCPServer::SetMessageCallback(const TCPServer::EventCallback &callback)
{
  message_callback_ = callback;
}
