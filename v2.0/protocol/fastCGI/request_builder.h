/**
 * Created on 12/22/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief Describe Class RequestBuilder
 */

#ifndef PLATINUM_FCGI_REQUEST_BUILDER_H
#define PLATINUM_FCGI_REQUEST_BUILDER_H

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "component.h"

namespace platinum {
namespace fcgi {

class BeginRequestRecord;
class RequestBuilder {
 public:
  RequestBuilder(int request_id,
                 int content_length,
                 const std::string &in,
                 const std::map<std::string, std::string> &params);

  bool Build();
  auto begin_requset() -> const BeginRequestRecord & { return begin_request_; }
  auto fcgi_params() -> const std::map<std::shared_ptr<FCGIData>, int> & {
    return fcgi_params_;
  }
  auto fcgi_in() -> const std::map<std::shared_ptr<FCGIData>, int> & {
    return fcgi_ins_;
  }
  auto empty_fcgi_param() -> const std::pair<std::shared_ptr<FCGIData>, int> & {
    return empty_fcgi_param_;
  }
  auto empty_fcgi_in() -> const std::pair<std::shared_ptr<FCGIData>, int> & {
    return empty_fcgi_in_;
  }



 private:
  void BuildParams();
  bool BuildParamImpl(const std::string &name, const std::string &value);
  bool BuildEmptyParam();
  void BuildStdins();
  bool BuildStdinImpl();
  bool BuildEmptyStdin();

  int request_id_;
  int content_length_;
  BeginRequestRecord begin_request_;
  std::string in_;
  std::map<std::string, std::string> params_;

  std::map<std::shared_ptr<FCGIData>, int> fcgi_params_;
  std::map<std::shared_ptr<FCGIData>, int> fcgi_ins_;
  std::pair<std::shared_ptr<FCGIData>, int> empty_fcgi_param_;
  std::pair<std::shared_ptr<FCGIData>, int> empty_fcgi_in_;
};

}
}

#endif //PLATINUM_FCGI_REQUEST_BUILDER_H
