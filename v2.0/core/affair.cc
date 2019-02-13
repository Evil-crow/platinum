/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/affair.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <utility>

#include "core/handler/handler.hpp"
#include "protocol/http/request_parser.h"
#include "utility/logger.h"
#include "config/config.h"

using namespace platinum;

Affair::Affair(platinum::Connection *connection, platinum::http::Request request) noexcept
    : connection_(connection),
      request_(std::move(request))
{
  ;
}

void Affair::Process()
{
  SetPathFile();
  SetSuffix();
  SetParameters();
}

void Affair::SetPathFile()
{
  auto url = request_.url();

  size_t end_pos = (url.find_first_of('?') != std::string::npos) ? url.find_first_of('?') : url.size();
  size_t begin_pos = url.find_last_of('/');
  file_ = std::string(url, begin_pos, end_pos);
  path_ = std::string(url, 0, begin_pos);
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

