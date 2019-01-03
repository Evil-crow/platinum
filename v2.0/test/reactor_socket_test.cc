//
// Created by Crow on 12/11/18.
//

#include <iostream>
#include <cstdio>
#include <memory>
#include <arpa/inet.h>

#include "net/socket.h"
#include "net/ip_address.h"
#include "net/acceptor.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"
#include "reactor/epoller.h"

using namespace platinum;

void func()
{
  std::cout << "I have callback readable event" << std::endl;
}

int main(int argc, char *argv[])
{

//   IPAddress + Socket
  IPAddress addr(9877);
  Socket acceptor(platinum::socket::INET);
  acceptor.SetTcpNoDelay(true);
  acceptor.SetReusePort(true);
  acceptor.Bind(addr);
  acceptor.Listen();

  EventLoop loop;
  auto channel = new Channel(&loop, acceptor.fd());
  channel->EnableReading();
  channel->EnableET();
  channel->SetReadCallback([]() { func(); });

  loop.AddChannel(channel);
  loop.Loop();

  return 0;
}
