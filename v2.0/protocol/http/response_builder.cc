/**
 * Created by Crow on 12/29/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "protocol/http/response_builder.h"

using namespace platinum::http;

ResponseBuilder::ResponseBuilder()
{
  status_code_ = "200";
}

const Response ResponseBuilder::GetResponse()
{
  return Response(version_major_, version_minor_, status_code_, body_, headers_);
}

void ResponseBuilder::SetVersionMajor(int version_major)
{
  version_major_ = std::to_string(version_major);
}

void ResponseBuilder::SetVersionMinor(int version_minor)
{
  version_minor_ = std::to_string(version_minor);
}

void ResponseBuilder::SetStatusCode(int status_code)
{
  status_code_ = std::to_string(status_code);
}

void ResponseBuilder::SetHeader(const std::string &key, const std::string &value)
{
  headers_.emplace(key, value);
}

void ResponseBuilder::SetBody(std::vector<unsigned char> body)
{
  body_.swap(body);
}
