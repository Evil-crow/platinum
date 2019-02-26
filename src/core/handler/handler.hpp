/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#ifndef PLATINUM_CORE_HANDLER_HANDLER_H
#define PLATINUM_CORE_HANDLER_HANDLER_H

#include <map>
#include <memory>

#include "include/net.hpp"
#include "include/http.hpp"

namespace platinum {

class Handler {
 public:
  explicit Handler() = delete;
  Handler(platinum::Connection *connection,
          http::Request request,
          std::string query_string,
          std::string file,
          std::string path) noexcept
      : connection_(connection),
        request_(std::move(request)),
        query_string_(std::move(query_string)),
        file_(std::move(file)),
        path_(std::move(path))
  { ; }

  ~Handler() = default;
  virtual void Serve(){};

 protected:
  void SetNewFile(std::string file) { file_ = std::move(file); }

  platinum::Connection *connection_;
  platinum::http::Request request_;
  std::string query_string_;
  std::string path_;
  std::string file_;
};

}

#endif //PLATINUM_CORE_HANDLER_HANDLER_H
