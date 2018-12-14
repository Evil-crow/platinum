//
// Created by Crow on 12/11/18.
//

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

void func(int fd, const IPAddress &addr)
{
  std::cout << "fd: " << fd << std::endl;
  std::cout << "ip: " << addr.port() << std::endl;
  std::cout << "port: " << addr.port() << std::endl;
}

int main(int argc, char *argv[])
{
  // Using Class Acceptor
  EventLoop loop;
  IPAddress addr(9877);
  Acceptor acceptor(&loop, addr);
  acceptor.SetConnectionCallback(func);
  acceptor.Listening();

  loop.Loop();

  return 0;
}
