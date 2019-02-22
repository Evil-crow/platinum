/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "protocol/http/response.h"

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace platinum::http;

Response::Response(const std::string &version_major,
                   const std::string &version_minor,
                   const std::string &status_code,
                   const std::vector<unsigned char> &body,
                   std::unordered_map<std::string, std::string> &headers)
    : version_major_(version_major),
      version_minor_(version_minor),
      status_code_(status_code),
      body_(body),
      headers_(headers)
{
  status_line_ += "HTTP/" + version_major_ + "." + version_minor_ + SetSpace();
  status_line_ += status_code_ + SetSpace();
  status_line_ += StatusDescription(status_code_) + SetCRLF();
}

auto Response::Build() -> std::vector<unsigned char>
{
  std::vector<unsigned char> data;
  std::string key_value_line;
  auto CRLF = SetCRLF();

  std::copy(status_line_.cbegin(), status_line_.cend(), std::back_inserter(data));
  for (const auto &header : headers_) {
    key_value_line += header.first + ": " + header.second + CRLF;
    std::copy(key_value_line.cbegin(), key_value_line.cend(), std::back_inserter(data));
    key_value_line.clear();
  }
  std::copy(CRLF.cbegin(), CRLF.cend(), std::back_inserter(data));

  if (!body_.empty()) {
    std::copy(body_.cbegin(), body_.cend(), std::back_inserter(data));
  }

  return data;
}

const std::string Response::StatusLine()
{
  return status_line_;
}

const std::string Response::SetCRLF()
{
  return {"\r\n"};
}

const std::string Response::SetSpace()
{
  return {" "};
}

