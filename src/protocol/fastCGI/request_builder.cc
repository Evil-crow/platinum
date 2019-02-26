/**
 * Created on 12/22/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief Only give the interface:
 *       RequestBuilder::Build()
 *       ReuestBuilder::getters
 */

#include "request_builder.h"

#include <cstring>
#include <algorithm>
#include <iostream>
#include <memory>
using namespace platinum::fcgi;

RequestBuilder::RequestBuilder(size_t request_id,
               int content_length,
               const std::string &in,
               const std::map<std::string, std::string> &params)
    : request_id_(request_id),
      content_length_(content_length),
      begin_request_(Role::FCGI_RESPONDER,
                    Base::FCGI_KEEP_CONN,
                    Type::FCGI_BEGIN_REQUEST,
                    request_id_,
                    8,
                    (content_length_ % 8)
                           ? (8 - (content_length_ % 8))
                           : 0),
      in_(in.cbegin(), in.cend()),
      params_(params)
{
  ;
}

bool RequestBuilder::Build()
{
  BuildParams();
  BuildStdins();

  return true;
}

void RequestBuilder::BuildParams()
{
  std::for_each(params_.cbegin(), params_.cend(), [this](std::map<std::string, std::string>::const_reference ref) {
    BuildParamImpl(ref.first, ref.second);
  });
  BuildEmptyParam();
}

bool RequestBuilder::BuildParamImpl(const std::string &name, const std::string &value)
{
  size_t name_len(name.size()), value_len(value.size());
  auto ct_len = name_len + value_len;

  name_len < 128 ? ct_len++ : ct_len += 4;
  value_len < 128 ? ct_len++ : ct_len += 4;
  auto pd_len = (ct_len % 8) ? (8 - (ct_len % 8)) : 0;
  auto total_len = Base::FCGI_HEADER_LEN + ct_len + pd_len;

  // Add Header for FCGI_PARAM
  Header header(Type::FCGI_PARAMS,
                request_id_,
                static_cast<int>(ct_len),
                static_cast<int>(pd_len));

  auto param_ptr = std::shared_ptr<FCGIData>(
      new FCGIData[total_len],
      std::default_delete<fcgi::FCGIData []>());
  auto temp(param_ptr.get());

  ::memcpy(temp, &header, Base::FCGI_HEADER_LEN);
  temp = temp + Base::FCGI_HEADER_LEN;

  if (name_len < 128) {
    *temp++ = static_cast<unsigned char>(name_len);
  } else {
    *temp++ = static_cast<unsigned char>((name_len >> 24) | 0x80);
    *temp++ = static_cast<unsigned char>(name_len >> 16);
    *temp++ = static_cast<unsigned char>(name_len >> 8);
    *temp++ = static_cast<unsigned char>(name_len);
  }
  if (value_len < 128) {
    *temp++ = static_cast<unsigned char>(value_len);
  } else {
    *temp++ = static_cast<unsigned char>((value_len >> 24) | 0x80);
    *temp++ = static_cast<unsigned char>(value_len >> 16);
    *temp++ = static_cast<unsigned char>(value_len >> 8);
    *temp++ = static_cast<unsigned char>(value_len);
  }

  ::memcpy(temp, name.c_str(), name_len);
  temp = temp + name_len;
  ::memcpy(temp, value.c_str(), value_len);

  // std::sahred_ptr<char>(param_ptr, [](char *ptr){ delete[] ptr;})      => too lao !
  fcgi_params_.insert({param_ptr, total_len});
  return true;
}

bool RequestBuilder::BuildEmptyParam()
{
  Header header(Type::FCGI_PARAMS,
                request_id_,
                0, 0);
  auto param_ptr = std::shared_ptr<FCGIData>(
      new FCGIData[Base::FCGI_HEADER_LEN],
      std::default_delete<fcgi::FCGIData []>());
  ::memcpy(param_ptr.get(), &header, Base::FCGI_HEADER_LEN);
  empty_fcgi_param_ = std::make_pair<std::shared_ptr<FCGIData>, int>(std::move(param_ptr), Base::FCGI_HEADER_LEN);
  return true;
}

void RequestBuilder::BuildStdins()
{
  BuildStdinImpl();
  BuildEmptyStdin();
}

bool RequestBuilder::BuildStdinImpl()
{
//  auto len = static_cast<int>(in_.size());

  auto len = content_length_;
  auto completed(0);
  while (len > 0) {
    auto ct_len = (len > Base::FCGI_MAX_LENGTH) ? Base::FCGI_MAX_LENGTH : len;
    auto pd_len = (ct_len % 8) ? (8 - ct_len % 8) : 0;
    auto total_len = Base::FCGI_HEADER_LEN + ct_len + pd_len;

    Header header(Type::FCGI_STDIN,
                  request_id_,
                  ct_len,
                  pd_len);
    auto in_ptr = std::shared_ptr<FCGIData>(
        new FCGIData[total_len],
        std::default_delete<fcgi::FCGIData []>());
    auto temp(in_ptr.get());

    ::memcpy(temp, &header, Base::FCGI_HEADER_LEN);
    temp = temp + Base::FCGI_HEADER_LEN;

    ::memcpy(temp, in_.c_str() + completed, ct_len);
    temp = temp + ct_len;

    ::memset(temp, 0, pd_len);
    temp = temp + pd_len;

    len = len - ct_len;
    completed = completed + ct_len;

    fcgi_ins_.insert({in_ptr, total_len});
  }

  return true;
}

bool RequestBuilder::BuildEmptyStdin()
{
  Header header(Type::FCGI_STDIN,
                request_id_,
                0, 0);
  auto in_ptr = std::shared_ptr<FCGIData>(
      new FCGIData[Base::FCGI_HEADER_LEN],
      std::default_delete<fcgi::FCGIData []>());
  ::memcpy(in_ptr.get(), &header, Base::FCGI_HEADER_LEN);
  empty_fcgi_in_ = std::make_pair<std::shared_ptr<FCGIData>, int>(std::move(in_ptr), Base::FCGI_HEADER_LEN);

  return true;
}
