/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This file is for FCGI request,
 *         Now ONLY SUPPORT FCGI protocol
 */

#ifndef PLATINUM_HANDLER_FCGI_HANDLER_H
#define PLATINUM_HANDLER_FCGI_HANDLER_H

#include <utility>
#include "core/handler/handler.hpp"
#include "include/fcgi.hpp"

namespace platinum {

class FCGIHandler : public Handler {
 public:
  FCGIHandler(Connection *connection,
              const http::Request &request,
              const std::string &query_string,
              const std::string &file,
              const std::string &path) noexcept;

  void Serve() override;

 private:
  fcgi::RequestBuilder BuildFCGIRequest();
  void StartFCGIConnection(const fcgi::RequestBuilder &builder);
};

namespace handler {

long FCGIOnMessage(Connection *conn, Buffer &buffer, std::unique_ptr<platinum::Parser> &parser);
const http::Response BuildResponse(const std::map<std::string, std::string> &kv);
void TransferData(std::shared_ptr<Connection> conn, const std::vector<unsigned char> &dt);

}

}

#endif //PLATINUM_HANDLER_FCGI_HANDLER_H
