/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "request.h"

#include <utility>

using namespace platinum::http;

Request::Request(std::string method,
                 std::string url,
                 int version_major,
                 int version_minor,
                 long body_len,
                 std::unordered_map<std::string, std::string> key_value_map,
                 std::vector<platinum::http::HttpData> body)
  : method_(std::move(method)),
    url_(std::move(url)),
    version_major_(version_major),
    version_minor_(version_minor),
    body_len_(body_len)
{
  key_value_map_.swap(key_value_map);
  body_.swap(body);
}

auto Request::headers() -> const std::unordered_map<std::string, std::string> &
{
  return key_value_map_;
}

const std::vector<platinum::http::HttpData> &Request::body() const
{
  return body_;
}

const std::string Request::header(const std::string &key)
{
  if (key_value_map_.find(key) != key_value_map_.end()) {
    return key_value_map_[key];
  } else {
    return std::string("");
  }
}

bool Request::HasBody()
{
  return header("Content-Length") != std::string("");
}

const std::string Request::status_line() const
{
  std::string status_line;

  status_line += method_;
  status_line += " ";
  status_line += url_;
  status_line += " ";
  status_line += "HTTP/";
  status_line += std::to_string(major_version());
  status_line += ".";
  status_line += std::to_string(minor_version());

  return status_line;
}

