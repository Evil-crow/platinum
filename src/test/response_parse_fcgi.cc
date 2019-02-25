/**
 * Created by Crow on 1/6/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This file use TcpServer to test FCGI Parse.
 */

#include <iostream>
#include "reactor/event_loop.h"
#include "net/tcp_server.h"
#include "net/connection.h"
#include "net/unix_address.h"
#include "utility/buffer/buffer.h"
#include "protocol/fastCGI/response_parser.h"
#include "protocol/fastCGI/request_builder.h"
#include "protocol/http/request.h"
#include "protocol/http/request_parser.h"

long FCGIOnMessage(platinum::Connection *conn, platinum::Buffer &buffer, std::unique_ptr<platinum::Parser> & parser)
{
  std::cout << "response" << '\n';
  std::vector<unsigned char> vec(buffer.BufferBegin(), buffer.BufferEnd());
  auto parser_ = static_cast<platinum::fcgi::ResponseParser *>(parser.get());
  auto len = parser_->Feed(vec.cbegin(), vec.size());
  auto stdout_ = parser_->transform_data();
  std::string str(stdout_.cbegin(), stdout_.cend());
  std::cout << str << std::endl;
  std::cout << str.size() << std::endl;
  if (parser_->Complete()) {
    auto id = parser_->request_id();
    auto app_status = parser_->app_status();
    auto status = parser_->status();
    std::cout << "id: " << id << std::endl;
    std::cout << "app: " << app_status << std::endl;
    std::cout << "status: " << status << std::endl;
    parser_->Reset();
    conn->ShutDownConnection();                             // FCGI controled by WebServer.
  }

  return len;
}


void func()
{
  std::map<std::string, std::string> param_map;
  param_map.insert({"REMOTE_PORT", "80"});
  param_map.insert({"REMOTE_ADDR", "127.0.0.1"});
  param_map.insert({"REQUEST_METHOD", "POST"});
  param_map.insert({"SERVER_PROTOCOL", "HTTP/1.1"});
  param_map.insert({"SCRIPT_FILENAME", "/home/Crow/1.php"});
  param_map.insert({"CONTENT_LENGTH", "11"});
  std::string in_str("a=b&c=d&e=f");
  platinum::fcgi::RequestBuilder builder(3, 11, in_str, param_map);

  builder.Build();

  auto b = builder.begin_requset();
  auto p = builder.fcgi_params();
  auto i = builder.fcgi_in();
  auto empty_p = builder.empty_fcgi_param();
  auto empty_i = builder.empty_fcgi_in();

  platinum::IPAddress addr(9000, "127.0.0.1");
//  platinum::UnixAddress addr("/run/php-fpm/www.sock");
//  std::cout << addr.path() << std::endl;
  printf("Create new connection to php-fpm\n");
  auto connect = platinum::TcpServer::NewConnector(addr, platinum::FCGI);
  printf("Set OK\n");
  connect->SetWriteCallback([=]() {
    std::cout << "Start send to fpm" << std::endl;
    connect->SendData((unsigned char *)(&b), sizeof(b));
    for (const auto &var : p) {
      connect->SendData(reinterpret_cast<unsigned char *>(var.first.get()), var.second);
    }
    connect->SendData(reinterpret_cast<unsigned char *>(empty_p.first.get()), empty_p.second);
    for (const auto &var : i) {
      connect->SendData(reinterpret_cast<unsigned char *>(var.first.get()), var.second);
    }
    connect->SendData(reinterpret_cast<unsigned char *>(empty_i.first.get()), empty_i.second);
//    connect->ShutdownConnection();
  });

  connect->SetMessageCallback(FCGIOnMessage);
  connect->StartNewConnection();
}


long HttpOnMessage(platinum::Connection *connection,
               const platinum::Buffer &buf,
               std::unique_ptr<platinum::Parser> &parser)
{
  std::vector<unsigned char> vec(buf.BufferBegin(), buf.BufferEnd());
  std::string str(buf.BufferBegin(), buf.BufferEnd());

  printf("Get message\n");
  auto parser_ = static_cast<platinum::http::RequestParser *>(parser.get());
  auto len = parser_->Feed(vec.cbegin(), vec.size());
  if (parser_->Complete()) {
//    std::cout << parser_->status_line() << std::endl;
    auto request = parser_->GetRequest();
    std::cout << request.status_line() << std::endl;
    func();
    parser_->Reset();
  }

  return len;
}


int main()
{
  platinum::IPAddress addr(9877);
  platinum::EventLoop loop;
  platinum::TcpServer server(&loop, addr);
  server.SetMessageCallback(HttpOnMessage);

  server.Start();
}