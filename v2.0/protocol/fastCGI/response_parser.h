/* 
 * Created by Crow on 12/26/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief: 
 */

#ifndef PLATINUM_RESPONSE_PARSER_H
#define PLATINUM_RESPONSE_PARSER_H

#include "component.h"

namespace fcgi {

enum State : int {
  COMPLETED,
  UNCOMPLETED,
  FAULT,
};

class ResponseParser {
 public:
  using const_iter = std::vector<FCGIData>::const_iterator;
  ResponseParser();
  ~ResponseParser() = default;

  bool feed(const_iter iter, int length);

  auto transform_data() -> const std::vector<FCGIData> & {
    return transform_data_;
  }
  int request_id()        { return request_id_; }
  int parser_pos()        { return parser_pos_; }
  long long app_status()  { return app_status_; }
  State state()           { return static_cast<State>(state_); }
  Status status()         { return static_cast<Status>(status_); }
  bool Complete()         { return complete_; }

 private:
  void ParserStdout(const_iter &iter, int &length, int ct_len, int pd_len);
  void ParserStderr(const_iter &iter, int &length, int ct_len, int pd_len);
  void ParserEndRequest(const_iter &iter);

  std::vector<FCGIData> transform_data_;
  std::vector<FCGIData> name_value_data_;
  int request_id_;
  int parser_pos_;
  int transform_len_;
  int padding_len_;
  long long app_status_;

  bool complete_;
  bool in_content_;
  State state_;
  Status status_;
};

}

#endif //PLATINUM_RESPONSE_PARSER_H
