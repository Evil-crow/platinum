//
// Created by Crow on 12/21/18.
//

#include <iostream>

#include <httpxx/Request.hpp>
#include <httpxx/Error.hpp>
#include <cstring>
#include "reactor/event_loop.h"
#include "net/connection.h"
#include "net/tcp_server.h"
#include "net/ip_address.h"
#include "utility/buffer/buffer.h"

bool OnMessage(platinum::Connection *connection, const platinum::Buffer &buf) {
  std::vector<char> vec(buf.BufferBegin(), buf.BufferEnd());
  std::string str(buf.BufferBegin(), buf.BufferEnd());
  std::cout << "str: " << str << std::endl;

  http::Request request;
  try {
    request.feed(vec.data(), vec.size());
    if (!request.complete()) {
      return false;
    }
  } catch (http::Error &e) {
    std::cout << e.what() << std::endl;
  }
  const char *message = "Parser HTTP Request OK!";
  connection->SendData(message, strlen(message));
//  connection->ShutDownConnection();
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