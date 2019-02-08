/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "affair.h"

#include <string>
#include <sstream>
#include <algorithm>

#include "protocol/http/request_parser.h"
#include "utility/logger.h"

using namespace platinum;

Affair::Affair(const platinum::http::Request &request)
    : request_(request)
{
  ;
}

void Affair::set_request_suffix()
{
  size_t end_pos(0);
  auto url = request_.url();

  end_pos = url.find_first_of('?');
  if (end_pos == std::string::npos)
    end_pos = url.size();
  auto dot_pos = url.find_last_of('.', end_pos);

  request_suffix_ = std::string(url, dot_pos + 1, end_pos);
}

void Affair::set_request_parameters()
{
  auto url = request_.url();
  auto method = request_.method();
  std::string parameter_data;

  if (method == "POST") {
    if (request_.header("Content-Type") == "application/json") {           // From JSON
      LOG(WARN) << "Affair::set_request_parameters() => Not Support Request PayLoad";
      status_code_ = 501;
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
    request_parameters_.emplace(key, value);
}