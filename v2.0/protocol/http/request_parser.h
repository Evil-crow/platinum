/**
 * Created by Crow on 12/29/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe Class RequestParser
 */

#ifndef PLATINUM_REQUEST_PARSER_H
#define PLATINUM_REQUEST_PARSER_H

#include <vector>
#include <unordered_map>

#include "protocol/http/base.h"
#include "protocol/parser.hpp"

namespace platinum {
namespace http {

class RequestParser : public platinum::Parser {
 public:
  using const_iter = std::vector<HttpData>::const_iterator;
  RequestParser();
  ~RequestParser() override = default;

  int feed(const_iter iter, int length);
  bool Complete() const;
  const std::string &status_line() const;
  const std::string &method() const             { return method_; }
  const std::string &url() const                { return url_; }
  const int major_version() const               { return version_major_; }
  const int minor_version() const               { return version_minor_; }

  auto headers() -> const std::unordered_map<std::string, std::string> & {
    return key_value_map_;
  }

  const std::string header(const std::string &key) {
    if (key_value_map_.find(key) != key_value_map_.end()) {
      return key_value_map_[key];
    } else {
      return std::string("");
    }
  }

  const std::vector<platinum::http::HttpData> &body() const {
    return body_;
  }

 private:
  bool HasBody();
  bool ParseStatusLine(const_iter &iter, int &length);
  bool ParseHeader(const_iter &iter, int &length);
  bool ParseBody(const_iter &iter, int &length);

  std::string method_;
  std::string url_;
  std::string key_;
  std::string value_;
  int version_major_;
  int version_minor_;
  long body_len_;
  std::unordered_map<std::string, std::string> key_value_map_;
  std::vector<platinum::http::HttpData> body_;

  bool complete_;
  HttpState http_state_;
  LineState line_state_;
  HeaderState header_state_;
};

}
}

#endif //PLATINUM_REQUEST_PARSER_H
