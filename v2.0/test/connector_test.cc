/**
 * Created by Crow on 1/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief This file is a test for Class Connector ,
 *       To connect this WS
 */


#include <cstring>
#include "net/connector.h"
#include "net/tcp_server.h"
#include "reactor/event_loop.h"
#include "utility/buffer/buffer.h"
#include "utility/logger.h"

#include <iostream>
#include <httpxx/Request.hpp>

bool OnMessage(platinum::Connection *connection, const platinum::Buffer &buf) {
  std::vector<unsigned char> vec(buf.BufferBegin(), buf.BufferEnd());
  std::string str(buf.BufferBegin(), buf.BufferEnd());
//  std::cout << "str: " << str << std::endl;
  http::Request request;

  request.feed(vec.data(), vec.size());
  if (!request.complete())
    return false;
  printf("Parse OK, to send\n");
  platinum::IPAddress addr(9877, "127.0.0.1");
  auto connect = platinum::GetCurrentServer()->NewConnector(addr);
  connect->SetWriteCallback([=]() {
    std::cout << str << std::endl;
    connect->SendData(str.c_str(), str.size());
//    connect->ShutdownConnection();
  });
  connect->StartNewConnection();
}

int main()
{
  platinum::EventLoop loop;
  platinum::IPAddress addr(9888);
  platinum::TcpServer server(&loop, addr);

  server.SetMessageCallback(OnMessage);
  server.Start();
}