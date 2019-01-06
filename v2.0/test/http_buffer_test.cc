//
// Created by Crow on 12/18/18.
//

#include <iostream>
#include "reactor/event_loop.h"
#include "net/connection.h"
#include "net/tcp_server.h"
#include "net/ip_address.h"
#include "protocol/http/request_parser.h"
#include "utility/buffer/buffer.h"

long OnMessage(platinum::Connection *connection,
               const platinum::Buffer &buf,
               std::unique_ptr<platinum::Parser> &parser)
{
  std::vector<unsigned char> vec(buf.BufferBegin(), buf.BufferEnd());
  std::string str(buf.BufferBegin(), buf.BufferEnd());
//  std::cout << "str: " << str << std::endl;

  printf("Get message\n");
//  std::cout << str << '\n';
  auto parser_ = static_cast<platinum::http::RequestParser *>(parser.get());
  auto len = parser_->feed(vec.cbegin(), vec.size());
  if (parser_->Complete()) {
    std::cout << parser_->status_line() << std::endl;
    parser_->Reset();
  }

  return len;
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