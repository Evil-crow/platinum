/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#ifndef PLATINUM_CORE_HANDLER_STATIC_HANDLER_H
#define PLATINUM_CORE_HANDLER_STATIC_HANDLER_H

#include "core/handler/handler.hpp"

#include "include/net.hpp"

namespace platinum {

class StaticHandler : public Handler {
 public:
  StaticHandler(Connection *connection,
                const http::Request &request,
                const std::string &query_string,
                const std::string &file,
                const std::string &path) noexcept ;
  void Serve() override;

 private:
  bool ServerByWrite();
  bool ServerBySendFile();
  void SetDateHeader();
  void SetConnectionHeader();
  void SetContentTypeHeader();
  void SetServerHeader();
  const char *MIMEType(const char *str);
  bool IsValid();
  bool IsExist();
  bool Forbidden();

  std::unique_ptr<http::ResponseBuilder> builder_;
  ssize_t file_size_;
};

}

#endif //PLATINUM_CORE_HANDLER_STATIC_HANDLER_H
