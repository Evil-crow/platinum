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

#ifndef PLATINUM_CORE_AFFAIR_H
#define PLATINUM_CORE_AFFAIR_H

#include <map>
#include <memory>

#include "include/net.hpp"
#include "include/http.hpp"

namespace platinum {

class Handler;
class Affair {
 public:
  Affair(platinum::Connection *connection, platinum::http::Request request) noexcept ;
  ~Affair() = default;

  void Serve();
  
 private:
  void set_handler();
  void set_path_file();
  void set_suffix();
  void set_query_stirng();

  auto suffix() -> const std::string { return suffix_; }
  auto query_string() -> const std::string { return query_string_; }
  bool IsDynamicResource();

  std::string suffix_;
  std::string path_;
  std::string file_;
  std::string query_string_;
  std::unique_ptr<Handler> handler_;
  platinum::Connection *connection_;
  platinum::http::Request request_;
};

long func(platinum::Connection *connection,
          const platinum::Buffer &buf,
          std::unique_ptr<platinum::Parser> &parser);

}

#endif //PLATINUM_CORE_AFFAIR_H