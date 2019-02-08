/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This file is for affair details.
 *        HTTP affair can be divided to static request + dynamic request
 *        So, the flow is:
 *              Server::OnMessage
 *                     ↓
 *                ParseRequest
 *                     ↓
 *        StaticResource/DynamicResource
 *                     ↓
 *      CloseCOnnection/Keep-Alive (Default: close)
 *                     ↓
 *                    END
 */

#ifndef PLATINUM_AFFAIR_H
#define PLATINUM_AFFAIR_H

#include <map>
#include "protocol/http/request_parser.h"

namespace platinum {

class Affair {
 public:
  explicit Affair(const platinum::http::Request &request);
  ~Affair() = default;

  void Process();
  void Serve();

 private:
  void set_request_suffix();
  void set_request_parameters();
  auto request_suffix() -> const std::string { return request_suffix_; }
  auto request_parameters() -> const std::map<std::string, std::string> { return request_parameters_; }

  bool IsStaticResource();
  bool IsDynamicResource();


  std::string request_suffix_;
  std::map<std::string, std::string> request_parameters_;
  platinum::http::Request request_;
  int status_code_;
};

}

#endif //PLATINUM_AFFAIR_H