/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#ifndef PLATINUM_CORE_HANDLER_STATIC_HANDLER_H
#define PLATINUM_CORE_HANDLER_STATIC_HANDLER_H

#include "core/handler/handler.hpp"

#include "net/connection.h"

namespace platinum {

class StaticHandler : public Handler {
 public:
  StaticHandler(Connection *connection,
                const http::Request &request,
                const std::unordered_map<std::string, std::string> &parameters,
                const std::string &file,
                const std::string &path) noexcept ;
  void Serve() override;
  void SetResourceFile();
  void SetBasicHeaders();

 private:
  bool IsValid();
  bool IsExist();
  bool NotForbidden();
};

}

#endif //PLATINUM_CORE_HANDLER_STATIC_HANDLER_H
