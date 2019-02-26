/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief
 */

#include "core/affair.h"

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <utility>

#include "core/handler/handler.hpp"
#include "core/handler/static_handler.h"
#include "core/handler/fcgi_handler.h"
#include "protocol/http/request_parser.h"
#include "utility/logger.h"
#include "config/config.h"

using namespace platinum;

Affair::Affair(platinum::Connection *connection, platinum::http::Request request) noexcept
    : connection_(connection),
      request_(std::move(request))
{
  set_path_file();
  set_suffix();
  set_query_stirng();
  set_handler();
}

void Affair::set_path_file()
{
  auto url = request_.url();

  size_t end_pos = (url.find_first_of('?') != std::string::npos) ? url.find_first_of('?') : url.size();
  size_t begin_pos = url.find_last_of('/');
  file_ = std::string(url, begin_pos + 1, end_pos - 1);
  path_ = std::string(url, 0, begin_pos);

  if (file_.empty()) {
    const auto &config = Config::GetInstance();
    file_ = config.index();
  }
}

void Affair::set_suffix()
{
  auto url = request_.url();

  size_t end_pos = (url.find_first_of('?') != std::string::npos) ? url.find_first_of('?') : url.size();
  auto dot_pos = url.find_last_of('.', end_pos);

  suffix_ = std::string(url, dot_pos + 1, end_pos);
}

void Affair::set_query_stirng()
{
  auto url = request_.url();
  auto method = request_.method();

  auto pos = url.find_first_of('?');
  if (pos == std::string::npos)
      query_string_ = std::string("");
  else
    query_string_ = std::string(url, pos + 1);
}

void Affair::Serve()
{
  handler_->Serve();
}

void Affair::set_handler()
{
  if (request_.method() == "POST") {           // dynamic
    handler_ = std::make_unique<FCGIHandler>(connection_, request_, query_string_, file_, path_);
  } else {
    if (!query_string_.empty()) {              // dynamic
      handler_ = std::make_unique<FCGIHandler>(connection_, request_, query_string_, file_, path_);
    } else {                                   // static
      handler_ = std::make_unique<StaticHandler>(connection_, request_, query_string_, file_, path_);
    }
  }
}

bool Affair::IsDynamicResource()
{
  const auto &config = Config::GetInstance();
  const auto &dynamic_resource = config.dynamic_resource();
  auto suffix = suffix_;

  return static_cast<bool>(dynamic_resource.count(suffix));
}

long platinum::func(platinum::Connection *connection,
                    const platinum::Buffer &buf,
                    std::unique_ptr<platinum::Parser> &parser)
{
  std::vector<unsigned char> vec(buf.BufferBegin(), buf.BufferEnd());
  std::string str(buf.BufferBegin(), buf.BufferEnd());

  auto http_parser = static_cast<platinum::http::RequestParser *>(parser.get());
  auto len = http_parser->Feed(vec.cbegin(), vec.size());
  if (http_parser->Complete()) {
    auto request = http_parser->GetRequest();
    platinum::Affair affair(connection, request);
    affair.Serve();
    http_parser->Reset();
  }

  return len;
}