/**
 * Created on 12/26/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief Class ResponseParser is used to parse FCGI Response
 */

#ifndef PLATINUM_FCGI_RESPONSE_PARSER_H
#define PLATINUM_FCGI_RESPONSE_PARSER_H

#include <map>
#include <string>

#include "base.h"
#include "protocol/fastCGI/component.h"
#include "protocol/parser.hpp"

namespace platinum {
namespace fcgi {

enum State : int {
  COMPLETED,
  UNCOMPLETED,
  FAULT,
};

 class ResponseParser : public platinum::Parser {
 public:
  using const_iter = std::vector<FCGIData>::const_iterator;
  ResponseParser();
  ~ResponseParser() override = default;

  long Feed(const_iter iter, long length);

  auto transform_data() -> const std::vector<FCGIData> & { return transform_data_; }
  auto key_value_() -> const std::map<std::string, std::string> & { return name_value_data_; }
  int request_id() { return request_id_; }
  long long app_status() { return app_status_; }
  State state() { return static_cast<State>(state_); }
  Status status() { return static_cast<Status>(status_); }
  bool Complete() { return complete_; }
  void Reset();

 private:
  void ParseStdout(const_iter &iter, long &length, long ct_len, long pd_len);
  void ParseStderr(const_iter &iter, long &length, long ct_len, long pd_len);
  void ParseEndRequest(const_iter &iter);

  std::vector<FCGIData> transform_data_;
  std::map<std::string, std::string> name_value_data_;
  int request_id_;
  long transform_len_;
  long padding_len_;
  long long app_status_;

  bool complete_;
  bool in_content_;
  State state_;
  Status status_;
};

}
}

#endif //PLATINUM_FCGI_RESPONSE_PARSER_H
