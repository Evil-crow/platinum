//
// Created by Crow on 12/12/18.
//

#include <iostream>

#include "reactor/event_loop.h"
#include "net/tcp_server.h"
#include "net/ip_address.h"

void OnMessage()
{
  std::cout << "I have got message from connfd" << std::endl;
}

int main()
{
  platinum::IPAddress addr(9877);
  platinum::EventLoop loop;
  platinum::TCPServer server(&loop, addr);

  server.SetMessageCallback(OnMessage);
  server.Start();

  return 0;
}