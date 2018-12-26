/**
 * Created by Crow on 12/26/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief This file is Class ResponseParser. It can be reentrant
 */

#include "response_parser.h"

#include <cstring>
#include <string>

using namespace fcgi;

fcgi::ResponseParser::ResponseParser()
    : request_id_(-1),
      parser_pos_(-1),
      transform_len_(0),
      padding_len_(0),
      app_status_(-1),
      complete_(false),
      in_content_(false),
      state_(State::UNCOMPLETED),
      status_(Status::FCGI_UNKNOWN_ROLE)
{
  transform_data_.reserve(1024);         // make sure reserve space for transform_data_
}

/**
 * @brief feed() the core to parse the FCGI response
 * @param iter  Buffer's cosnt iterator
 * @param length Buffer's length this time
 * @return parse result
 */

bool ResponseParser::feed(ResponseParser::const_iter iter, int length)
{
  // To ensure the last parsing result is complete
  if (transform_len_) {
    auto len = transform_len_ > length ? length : transform_len_;
    transform_data_.insert(transform_data_.cend(), iter, iter + len);
    length -= len;                                                     // reduce the length
    iter += len;                                                       // move the iter
    transform_len_ -= len;
  }

  if (length == 0) {
    return true;
  } else if (padding_len_) {
    auto len = padding_len_ > length ? length : padding_len_;
    length -= len;
    iter += len;
    padding_len_ -= len;
  }

  while (length) {    // the whole parsing process continus utils length < 0
    if (state_ == State::COMPLETED) {
      return true;
    } else if (state_ == State::FAULT) {
      return true;
    }

    if (length == 0) {
      return true;
    } else if (length < sizeof(Header)) {
      parser_pos_ = length - transform_len_;                                        // transform_len_ mustn't consume buffer completely
      return false;
    }

    Header header(iter);                                                          // Construct a header
    request_id_ = header.request_id();
    auto ct_len = header.content_length();
    auto pd_len = header.padding_length();
    switch (header.type()) {
      case Type::FCGI_STDOUT: ParserStdout(iter, length, ct_len, pd_len); break;
      case Type::FCGI_STDERR: ParserStderr(iter, length, ct_len, pd_len); break;
      case Type::FCGI_END_REQUEST: ParserEndRequest(iter, length); break;
      default: break;
    }
  }
}

void ResponseParser::ParserStdout(const_iter &iter, int &length, int ct_len, int pd_len)
{
  if (ct_len == 0 && pd_len == 0)
    return ;
  auto len1 = ct_len > length ? length : ct_len;                   // judge if we have enough space to deal with

  std::string str(iter, iter + len1);
  std::string::size_type pos;
  if (!in_content_) {
    if ((pos = str.find("\r\n\r\n")) != std::string::npos) {
      name_value_data_.insert(name_value_data_.cend(), iter, iter + pos);
      iter += pos + 4;
      length -= pos;
      ct_len -= pos;
      len1 -= pos;
    } else {
      state_ = State::FAULT;
      complete_ = true;
      return ;
    }
    in_content_ = true;
  }

  transform_data_.insert(transform_data_.cend(), iter, iter + len1);
  iter += len1;
  length -= len1;
  ct_len -= len1;

  if (length == 0) {
    transform_len_ += ct_len + pd_len;
    padding_len_ = pd_len;
    return ;
  }

  auto len2 = pd_len > length ? length : pd_len;
  iter += len2;
  length -= len2;
  pd_len -= len2;

  if (length == 0) {
    padding_len_ = pd_len;
    return ;
  }
}

void ResponseParser::ParserStderr(const_iter &iter, int &length, int ct_len, int pd_len)
{
  if (ct_len == 0 && pd_len == 0)
    return ;
  auto len1 = ct_len > length ? length : ct_len;                   // judge if we have enough space to deal with

  transform_data_.insert(transform_data_.cend(), iter, iter + len1);
  iter += len1;
  length -= len1;
  ct_len -= len1;

  if (length == 0) {
    transform_len_ += ct_len + pd_len;
    padding_len_ = pd_len;
    return ;
  }

  auto len2 = pd_len > length ? length : pd_len;
  iter += len2;
  length -= len2;
  pd_len -= len2;

  if (length == 0) {
    padding_len_ = pd_len;
    return ;
  }
}

void ResponseParser::ParserEndRequest(const_iter &iter, int &length)
{
  iter -= sizeof(Header);
  EndRequestRocord end_request_record(iter);

  app_status_ = end_request_record.app_status();
  status_ = end_request_record.protocol_status();

  complete_ = true;
  state_ = State::COMPLETED;
}