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
  SetPathFile();
  SetSuffix();
  SetParameters();
  SetHandler();
}

void Affair::SetPathFile()
{
  auto url = request_.url();

  size_t end_pos = (url.find_first_of('?') != std::string::npos) ? url.find_first_of('?') : url.size();
  size_t begin_pos = url.find_last_of('/');
  file_ = std::string(url, begin_pos + 1, end_pos);
  path_ = std::string(url, 0, begin_pos);

  if (file_.empty()) {
    const auto &config = Config::GetInstance();
    file_ = config.index();
  }
}

void Affair::SetSuffix()
{
  auto url = request_.url();

  size_t end_pos = (url.find_first_of('?') != std::string::npos) ? url.find_first_of('?') : url.size();
  auto dot_pos = url.find_last_of('.', end_pos);

  suffix_ = std::string(url, dot_pos + 1, end_pos);
}

void Affair::SetParameters()
{
  auto url = request_.url();
  auto method = request_.method();
  std::string parameter_data;

  if (method == "POST") {
    if (request_.header("Content-Type") == "application/json") {           // From JSON
      LOG(WARN) << "Affair::set_request_parameters() => Not Support Request PayLoad";
      handler_->SetStatusCode(501);
    } else {                                                               // From Data
      parameter_data = std::string(request_.body().cbegin(), request_.body().cend());
    }
  } else {                                                                 // method GET/HEAD, are always From Data
    auto pos = url.find_first_of('?');
    if (pos == std::string::npos)
      return ;
    parameter_data = std::string(url, pos + 1);
  }

  for (auto &i : parameter_data) {
    if (i == '&' || i == '=')
      i = ' ';
  }

  std::istringstream is(parameter_data);
  std::string key, value;
  while (is >> key >> value)
    parameters_.emplace(key, value);
}

void Affair::Serve()
{
  handler_->Serve();
}

void Affair::SetHandler()
{
  if (IsDynamicResource()) {
    if (Suffix() == "php") {
      handler_ = std::make_unique<FCGIHandler>(connection_, request_, parameters_, file_, path_);
    }
  } else {
    handler_ = std::make_unique<StaticHandler>(connection_, request_, parameters_, file_, path_);
  }
}

bool Affair::IsStaticResource()
{
  const auto &config = Config::GetInstance();
  const auto &static_resource = config.static_resource();
  auto suffix = Suffix();

  return static_cast<bool>(static_resource.count(suffix));
}

bool Affair::IsDynamicResource()
{
  const auto &config = Config::GetInstance();
  const auto &dynamic_resource = config.dynamic_resource();
  auto suffix = Suffix();

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