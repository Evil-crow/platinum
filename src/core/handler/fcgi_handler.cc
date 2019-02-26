/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/handler/fcgi_handler.h"

#include <iostream>
#include <unistd.h>

#include "net/tcp_server.h"
#include "net/unix_address.h"
#include "protocol/fastCGI/request_builder.h"
#include "protocol/fastCGI/response_parser.h"
#include "utility/logger.h"

using namespace platinum;

FCGIHandler::FCGIHandler(platinum::Connection *connection,
                         const platinum::http::Request &request,
                         const std::string &query_string,
                         const std::string &file,
                         const std::string &path) noexcept
    : Handler(connection, request, query_string, file, path)
{
  ;
}

void FCGIHandler::Serve()
{
  auto builder = BuildFCGIRequest();
  builder.Build();

  StartFCGIConnection(builder);
}



fcgi::RequestBuilder FCGIHandler::BuildFCGIRequest()
{
  const auto &config = Config::GetInstance();
  auto fcgi_dir = config.fcgi_root();

  auto script_name = fcgi_dir + "/" + file_;
  std::string body(request_.body().cbegin(), request_.body().cend());
  std::map<std::string, std::string> param_map;
  param_map.emplace("REQUEST_METHOD", request_.method());
  param_map.emplace("SCRIPT_FILENAME", script_name);
  param_map.emplace("CONTENT_LENGTH", std::to_string(body.size()));

  if (request_.method() == "POST") {                                            // method: POST
    param_map.emplace("CONTENT_TYPE", "application/x-www-form-urlencoded");
    //TODO: application/from-data, RequestPaylod
  } else {                                                                     // method: GET
    param_map.emplace("QUERY_STRING", query_string_);
  }

  return fcgi::RequestBuilder(static_cast<size_t>(connection_->socket_fd()), static_cast<int>(body.size()), body, param_map);
}

void FCGIHandler::StartFCGIConnection(const fcgi::RequestBuilder &builder)
{
  const auto &config = Config::GetInstance();
  auto fcgi_dir = config.fcgi_root();

  auto begin_request = builder.begin_requset();
  auto params = builder.fcgi_params();
  auto in = builder.fcgi_in();
  auto empty_param = builder.empty_fcgi_param();
  auto empty_in = builder.empty_fcgi_in();

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
  connector->set_write_callback([=]() {
    connector->SendData((unsigned char *)(&begin_request), sizeof(begin_request));
    for (const auto &var : params)
      connector->SendData(var.first.get(), static_cast<size_t>(var.second));
    connector->SendData(empty_param.first.get(), static_cast<size_t>(empty_param.second));
    for (const auto &var : in)
      connector->SendData(var.first.get(), static_cast<size_t>(var.second));
    connector->SendData(empty_in.first.get(), static_cast<size_t>(empty_in.second));
  });

  connector->set_message_callback(platinum::handler::FCGIOnMessage);
  connector->StartNewConnection();
}

long handler::FCGIOnMessage(platinum::Connection *conn, platinum::Buffer &buffer, std::unique_ptr<platinum::Parser> &p)
{
  auto parser = static_cast<platinum::fcgi::ResponseParser *>(p.get());
  auto size = buffer.BufferEnd() - buffer.BufferBegin();
  auto len = parser->Feed(buffer.BufferBegin(), size);
  auto data = parser->transform_data();
  auto forward_connection = TcpServer::ForwawrdConnection(conn->forward_fd());

  if (!parser->key_value().empty()) {                                           // send resposne header
    auto resposne = handler::BuildResponse(parser->key_value());
    auto response_header = resposne.Build();
    forward_connection->SendData(response_header.data(), response_header.size());
  }

  // transform the response from FCGI to HTTP Client

  handler::TransferData(forward_connection, data);
  if (parser->End()) {
    std::vector<unsigned char> zero;
    handler::TransferData(forward_connection, zero);
  }

  if (parser->Complete()) {
    parser->Reset();
    conn->ShutDownConnection();                         // webserver control the connection with FCGI peer (e.g. php-fpm)

    // TODO: HTTP/1.1 keep-alive the timer.
  }

  return len;
}

const http::Response handler::BuildResponse(const std::map<std::string, std::string> &kv)
{
  http::ResponseBuilder builder;

  builder.SetVersionMajor(1);
  builder.SetVersionMinor(1);

  // Header: Date
  char time[64];                      // 64 Bytes is already enough !
  auto t = std::time(nullptr);
  std::strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S GMT",std::localtime(&t));
  builder.SetHeader("Date", time);

  // Header: Connection
  builder.SetHeader("Connection", "keep-alive");        // HTTP/1.1 default

  // Header: Server
  builder.SetHeader("Server", "platinum/2.0");

  // Header: Transfer-Encoding
  builder.SetHeader("Transfer-Encoding", "chunked");

  // Header: From FCGI peer
  for (const auto &var : kv)
      builder.SetHeader(var.first, var.second);
  if (!kv.count("Status"))
    builder.SetStatusCode(200);                         // default status code is 200 (OK)

  return builder.GetResponse();
}

void handler::TransferData(std::shared_ptr<Connection> conn, const std::vector<unsigned char> &dt)
{
  std::vector<unsigned char> data;
  auto size = std::to_string(dt.size());
  std::string CRLF("\r\n");
  std::copy(size.cbegin(), size.cend(), back_inserter(data));
  std::copy(CRLF.cbegin(), CRLF.cend(), back_inserter(data));
  std::copy(dt.cbegin(), dt.cend(), back_inserter(data));
  std::copy(CRLF.cbegin(), CRLF.cend(), back_inserter(data));
  conn->SendData(data.data(), data.size());
}