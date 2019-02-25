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

#include "net/connection.h"
#include "protocol/http/request.h"
#include "protocol/http/response_builder.h"

namespace platinum {

class Handler {
 public:
  explicit Handler() = delete;
  Handler(platinum::Connection *connection,
          http::Request request,
          std::map<std::string, std::string> parameters,
          std::string file,
          std::string path) noexcept
      : connection_(connection),
        request_(std::move(request)),
        parameters_(std::move(parameters)),
        builder_(std::make_unique<http::ResponseBuilder>()),
        file_(std::move(file)),
        path_(std::move(path))
  { ; }

  ~Handler() = default;

  void SetStatusCode(int status_code) { builder_->SetStatusCode(status_code); }
  virtual void Serve(){};

 protected:
  void SetNewFile(std::string file) { file_ = std::move(file); }

  platinum::Connection *connection_;
  platinum::http::Request request_;
  std::map<std::string, std::string> parameters_;
  std::unique_ptr<http::ResponseBuilder> builder_;
  std::string path_;
  std::string file_;
};

}

#endif //PLATINUM_CORE_HANDLER_HANDLER_H
