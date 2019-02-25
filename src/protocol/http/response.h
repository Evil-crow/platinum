/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#ifndef PLATINUM_HTTP_RESPONSE_H
#define PLATINUM_HTTP_RESPONSE_H

#include <string>
#include <vector>
#include <unordered_map>

namespace platinum {
namespace http {

const static  std::unordered_map<std::string, std::string> HTTP_STATUS_MAP = {
    {"200", "OK"},
    {"400", "Bad Request"},
    {"403", "Forbidden"},
    {"404", "Not Found"},
    {"411", "Length Required"},
    {"500", "Internal Server Error"},
    {"501", "Not Implemented"},
};

#define StatusDescription(status_code) HTTP_STATUS_MAP.at(status_code)

class Response {
 public:
  Response(const std::string &version_major,
           const std::string &version_minor,
           const std::string &status_code,
           const std::vector<unsigned char> &body,
           std::unordered_map<std::string, std::string> &headers);
  ~Response() = default;
  std::vector<unsigned char> Build();
  const std::string StatusLine();

 private:
  const std::string SetCRLF();
  const std::string SetSpace();

  std::string version_major_;
  std::string version_minor_;
  std::string status_code_;
  std::string status_line_;
  std::unordered_map<std::string, std::string> headers_;
  std::vector<unsigned char> body_;
};

}
}

#endif //PLATINUM_HTTP_RESPONSE_H
