/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/handler/fcgi_handler.h"

#include <iostream>

#include "net/tcp_server.h"
#include "net/unix_address.h"
#include "protocol/fastCGI/request_builder.h"
#include "protocol/fastCGI/response_parser.h"
#include "utility/logger.h"

using namespace platinum;

FCGIHandler::FCGIHandler(platinum::Connection *connection,
                         const platinum::http::Request &request,
                         const std::map<std::string, std::string> &parameters,
                         const std::string &file,
                         const std::string &path) noexcept
    : Handler(connection, request, parameters, file, path)
{
  ;
}

void FCGIHandler::Serve()
{
  const auto &config = Config::GetInstance();
  auto fcgi_dir = config.fcgi_root();

  auto script_name = fcgi_dir + "/" + file_;
//  std::string body(request_.body().cbegin(), request_.body().cend());
  std::string body("a=1&b=2&c=3");
  std::map<std::string, std::string> param_map;
//  param_map.emplace("REQUEST_METHOD", request_.method());
//  param_map.emplace("SERVER_PROTOCOL", "HTTP/1.1");
//  param_map.emplace("SCRIPT_NAME", script_name);
  param_map.insert({"REMOTE_PORT", "80"});
  param_map.insert({"REMOTE_ADDR", "127.0.0.1"});
  param_map.insert({"REQUEST_METHOD", "POST"});
  param_map.insert({"SERVER_PROTOCOL", "HTTP/1.1"});
  param_map.insert({"SCRIPT_FILENAME", "/home/Crow/1.php"});
  if (!body.empty()) {
    param_map.emplace("CONTENT_LENGTH", std::to_string(request_.body().size()));
  }
  platinum::fcgi::RequestBuilder builder(9, static_cast<int>(body.size()), body, param_map);

  builder.Build();
  auto b = builder.begin_requset();
  auto p = builder.fcgi_params();
  auto i = builder.fcgi_in();
  auto empty_p = builder.empty_fcgi_param();
  auto empty_i = builder.empty_fcgi_in();

  std::shared_ptr<platinum::Connector> connector{nullptr};
  if (config.IsInetAddr()) {
    auto ip = config.fcgi_inet_ip();
    auto port = config.fcgi_inet_port();
    platinum::IPAddress addr(port, ip);
    connector = platinum::TcpServer::NewConnector(addr, platinum::FCGI);
  } else {
    auto unix_addr = config.fcgi_unix();
    platinum::UnixAddress addr(unix_addr);
    connector = platinum::TcpServer::NewConnector(addr, platinum::FCGI);
  }
  connector->set_forward_fd(connection_->socket_fd());
  connector->SetWriteCallback([=]() {
    connector->SendData((unsigned char *)(&b), sizeof(b));
    for (const auto &var : p)
      connector->SendData(var.first.get(), static_cast<size_t>(var.second));
    connector->SendData(empty_p.first.get(), static_cast<size_t>(empty_p.second));
    for (const auto &var : i)
      connector->SendData(var.first.get(), static_cast<size_t>(var.second));
    connector->SendData(empty_i.first.get(), static_cast<size_t>(empty_i.second));
  });

  connector->SetMessageCallback(platinum::FCGIOnMessage);
  connector->StartNewConnection();
}

long platinum::FCGIOnMessage(platinum::Connection *conn, platinum::Buffer &buffer, std::unique_ptr<platinum::Parser> &p)
{
  std::cout << "response" << '\n';
  std::vector<unsigned char> vec(buffer.BufferBegin(), buffer.BufferEnd());
  auto parser = static_cast<platinum::fcgi::ResponseParser *>(p.get());
  auto len = parser->Feed(vec.cbegin(), vec.size());
  auto stdout_ = parser->transform_data();
  std::string str(stdout_.cbegin(), stdout_.cend());
  std::cout << str << std::endl;
  std::cout << str.size() << std::endl;

  auto forward_connection = TcpServer::ForwawrdConnection(conn->forward_fd());
  if (!parser->key_value_().empty()) {                      // The first packet, should send resposne header
    platinum::http::ResponseBuilder builder;
    builder.SetVersionMajor(1);
    builder.SetVersionMinor(1);
    builder.SetStatusCode(200);
    builder.SetHeader("Date", "adsadadada");
    builder.SetHeader("Server", "platinum/2.0");
    builder.SetHeader("Transfer-Encoding", "chunked");
    for (const auto &var : parser->key_value_())
      builder.SetHeader(var.first, var.second);
    auto resposne = builder.GetResponse();
    auto data = resposne.Build();
    forward_connection->SendData(data.data(), data.size());
  }
                                                            // transform the response from FCGI to HTTP Client
  if (!stdout_.empty()) {
    std::vector<unsigned char> stdout_data;
    std::string s(std::to_string(stdout_.size()));
    std::string CRLF("\r\n");
    std::copy(s.cbegin(), s.cend(), back_inserter(stdout_data));
    std::copy(CRLF.cbegin(), CRLF.cend(), back_inserter(stdout_data));
    std::copy(stdout_.cbegin(), stdout_.cend(), back_inserter(stdout_data));
    std::copy(CRLF.cbegin(), CRLF.cend(), back_inserter(stdout_data));
    forward_connection->SendData(stdout_data.data(), stdout_data.size());
    std::string sv(stdout_.cbegin(), stdout_.cend());
    std::cout << sv << std::endl;
  }
  if (parser->Complete()) {
    auto id = parser->request_id();
    auto app_status = parser->app_status();
    auto status = parser->status();
    std::cout << "id: " << id << std::endl;
    std::cout << "app: " << app_status << std::endl;
    std::cout << "status: " << status << std::endl;
    parser->Reset();
//    conn->ShutDownConnection();                             // FCGI controled by WebServer.
  }

  return len;
}

// Header: Date
void FCGIHandler::SetDateHeader()
{
  char time[64];                      // 64 Bytes is already enough !
  auto t = std::time(nullptr);
  std::strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S GMT",std::localtime(&t));

  builder_->SetHeader("Date", time);
}

// Header: Connection
void FCGIHandler::SetConnectionHeader()
{
  builder_->SetHeader("Connection", request_.header("Connection"));
}

// Header: Server
void FCGIHandler::SetServerHeader()
{
  builder_->SetHeader("Server", "platinum/2.0");
}

