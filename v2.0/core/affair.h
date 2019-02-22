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

#include "net/connection.h"
#include "protocol/http/request_parser.h"

namespace platinum {

class Handler;
class Affair {
 public:
  Affair(platinum::Connection *connection, platinum::http::Request request) noexcept ;
  ~Affair() = default;

  void Serve();
  
 private:
  void Process();
  void SetHandler();
  void SetPathFile();
  void SetSuffix();
  void SetParameters();

  auto Suffix() -> const std::string { return suffix_; }
  auto Parameters() -> const std::map<std::string, std::string> { return parameters_; }
  bool IsStaticResource();
  bool IsDynamicResource();

  std::string suffix_;
  std::string path_;
  std::string file_;
  std::map<std::string, std::string> parameters_;
  std::unique_ptr<Handler> handler_;
  platinum::Connection *connection_;
  platinum::http::Request request_;
};

long func(platinum::Connection *connection,
          const platinum::Buffer &buf,
          std::unique_ptr<platinum::Parser> &parser);

}

#endif //PLATINUM_CORE_AFFAIR_H