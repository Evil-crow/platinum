//
// Created by Crow on 12/12/18.
//

#include <iostream>

#include "reactor/event_loop.h"
#include "net/tcp_server.h"
#include "net/ip_address.h"
#include "utility/buffer/buffer.h"

bool OnMessage(platinum::Connection *connection, const platinum::Buffer &buf)
{
  std::cout << "I have got message from connfd" << std::endl;
}

int main()
{
  platinum::IPAddress addr(9877);
  platinum::EventLoop loop;
  platinum::TcpServer server(&loop, addr);

  server.SetMessageCallback(OnMessage);
  server.Start();

  return 0;
}