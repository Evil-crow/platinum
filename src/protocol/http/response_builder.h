/**
 * Created by Crow on 12/29/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#ifndef PLATINUM_HTTP_RESPONSE_BUILDER_H
#define PLATINUM_HTTP_RESPONSE_BUILDER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "protocol/http/response.h"

namespace platinum {
namespace http {

class ResponseBuilder {
 public:
  ResponseBuilder();

  const Response GetResponse();
  void SetVersionMajor(int version_major);
  void SetVersionMinor(int version_minor);
  void SetStatusCode(int status_code);
  void SetHeader(const std::string &key, const std::string &value);
  void SetBody(std::vector<unsigned char> body);

 private:
  std::string version_major_;
  std::string version_minor_;
  std::string status_code_;
  std::unordered_map<std::string, std::string> headers_;
  std::vector<unsigned char> body_;
};

}
}

#endif //PLATINUM_RESPONSE_BUILDER_H
