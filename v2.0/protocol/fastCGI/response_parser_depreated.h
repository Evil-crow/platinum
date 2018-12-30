/* 
 * Created by Crow on 12/24/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief:  This file is used to describe the ResponseBuilder.
 *         you can use it as the follow step:
 *
 */

#ifndef PLATINUM_RESPONSE_PARSER_H
#define PLATINUM_RESPONSE_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "base.h"

namespace fcgi {

enum ParserState : int {
  STDOUT,
  STDERR,
  ENDREQUEST,
};

class ResponseParser {
 public:
  using const_iter = std::vector<FCGIData>::const_iterator;
  ResponseParser();
  ~ResponseParser() = default;

  long feed(const std::vector<FCGIData> &data);
  bool complete();

  int request_id() { return request_id_; }
  auto fcgi_out() -> std::vector<std::pair<std::shared_ptr<FCGIData>, int>> {
    return fcgi_out_;
  };
  auto fcgi_err() -> std::vector<std::pair<std::shared_ptr<FCGIData>, int>> {
    return fcgi_err_;
  }

 private:
  void ParserStdin(const_iter iter, int ct_len);
  void ParserNVPairs(const_iter iter_begin, const_iter iter_end);
  void ParserStderr(const_iter iter, int ct_len);
  std::vector<std::string> NVPairs;
  std::vector<std::pair<std::shared_ptr<FCGIData>, int>> fcgi_out_;
  std::vector<std::pair<std::shared_ptr<FCGIData>, int>> fcgi_err_;

  int request_id_;
  bool complete_;
  long long app_status_;
  State state_;
  ParserState parser_state_;
};

}

#endif //PLATINUM_RESPONSE_PARSER_H
