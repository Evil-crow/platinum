/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class is for build Request for HTTP affair
 *         RequestParser is an parser, not a request
 */

#ifndef PLATINUM_HTTP_REQUEST_H
#define PLATINUM_HTTP_REQUEST_H

#include <vector>
#include <unordered_map>

#include "protocol/http/base.h"

namespace platinum {
namespace http {

class Request {
 public:
  Request(std::string method,
          std::string url,
          int version_major,
          int version_minor,
          long body_len,
          std::unordered_map<std::string, std::string> key_value_map,
          std::vector<platinum::http::HttpData> body);

  const std::string status_line() const;
  const std::string &method() const             { return method_; }
  const std::string &url() const                { return url_; }
  const int major_version() const               { return version_major_; }
  const int minor_version() const               { return version_minor_; }

  auto headers() -> const std::unordered_map<std::string, std::string> &;
  const std::string header(const std::string &key);
  const std::vector<platinum::http::HttpData> &body() const;

 private:
  bool HasBody();

  std::string method_;
  std::string url_;
  int version_major_;
  int version_minor_;
  long body_len_;
  std::unordered_map<std::string, std::string> paramter_argument_map_;
  std::unordered_map<std::string, std::string> key_value_map_;
  std::vector<platinum::http::HttpData> body_;
};

}
}

#endif //PLATINUM_HTTP_REQUEST_H
