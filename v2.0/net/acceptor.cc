//
// Created by Crow on 11/25/18.
//

#include "acceptor.h"

#include "utility/logger.h"

using namespace platinum;

Acceptor::Acceptor(in_port_t port)
    : address_(port)
{
  ;
}

Acceptor::Acceptor(in_port_t port, const std::string &ip)
    : address_(port, ip)
{
  ;
}

void Acceptor::Listening()
{
  if (listenfd_.SetKeepAlive(true)
      && listenfd_.SetReusePort(true)
      && listenfd_.SetTcpNoDelay(true)) {
    LOG(INFO) << "Acceptor Listening OK!";
  } else {
    LOG(ERROR) << "Acceptor Listening ERROR!";
  }

  listenfd_.Bind(address_);
  listenfd_.Listen();
}

TCPConnection Acceptor::AcceptConnection()
{


  return
}


