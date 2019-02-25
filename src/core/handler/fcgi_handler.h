/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This file is for FCGI request,
 *         Now ONLY SUPPORT FCGI protocol
 */

#ifndef PLATINUM_HANDLER_FCGI_HANDLER_H
#define PLATINUM_HANDLER_FCGI_HANDLER_H

#include "core/handler/handler.hpp"
#include <utility>

namespace platinum {

class FCGIHandler : public Handler {
 public:
  FCGIHandler(Connection *connection,
              const http::Request &request,
              const std::map<std::string, std::string> &parameters,
              const std::string &file,
              const std::string &path) noexcept ;

  void Serve() override;

 private:
  void SetDateHeader();
  void SetConnectionHeader();
  void SetServerHeader();
};

long FCGIOnMessage(Connection *conn, Buffer &buffer, std::unique_ptr<platinum::Parser> & parser);
}

#endif //PLATINUM_HANDLER_FCGI_HANDLER_H
