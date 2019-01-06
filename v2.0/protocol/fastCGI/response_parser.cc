/**
 * Created by Crow on 12/26/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief This file is Class ResponseParser. It can be reentrant
 */

#include "response_parser.h"

#include <cstring>
#include <string>

using namespace platinum::fcgi;

ResponseParser::ResponseParser()
    : request_id_(-1),
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

long ResponseParser::feed(ResponseParser::const_iter iter, long length)
{
  auto len_temp(length);
  transform_data_.clear();
  // To ensure the last parsing result is complete
  if (transform_len_) {
    auto len = transform_len_ > length ? length : transform_len_;
    transform_data_.insert(transform_data_.cend(), iter, iter + len);
    length -= len;                                                     // reduce the length
    iter += len;                                                       // move the iter
    transform_len_ -= len;
  }

  if (length == 0) {
    return (len_temp - length);
  } else if (padding_len_) {
    auto len = padding_len_ > length ? length : padding_len_;
    length -= len;
    iter += len;
    padding_len_ -= len;
  }

  while (length) {    // the whole parsing process continus utils length < 0
    if (state_ == State::COMPLETED
        || state_ == State::FAULT
        || length < sizeof(Header))
    {
      return (len_temp - length);
    }

    Header header(iter);                                                           // Construct a header
    iter += sizeof(Header);
    length -= sizeof(Header);
    request_id_ = header.request_id();
    auto ct_len = header.content_length();
    auto pd_len = header.padding_length();
    switch (header.type()) {
      case Type::FCGI_STDOUT: ParseStdout(iter, length, ct_len, pd_len); break;
      case Type::FCGI_STDERR: ParseStderr(iter, length, ct_len, pd_len); break;
      case Type::FCGI_END_REQUEST: ParseEndRequest(iter); break;
      default: break;
    }
  }

  return len_temp - length;
}

/**
 * @brief To parse the STDOUT part
 * @param iter buffer's iterator (ref)
 * @param length buffer's length (ref)
 * @param ct_len the content length of FCGI_STDOUT
 * @param pd_len the padding length of FCGI_STDOUT
 */
void ResponseParser::ParseStdout(const_iter &iter, long &length, long ct_len, long pd_len)
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
      length -= pos + 4;
      ct_len -= pos + 4;
      len1 -= pos + 4;
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
    transform_len_ += ct_len;
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

/**
 * @beief To parse the STDERR part
 * @param iter buffer's iterator (ref)
 * @param length buffer's length (ref)
 * @param ct_len the conten length of FCGI_STDERR
 * @param pd_len the padding length of FCGI_STDERR
 */

void ResponseParser::ParseStderr(const_iter &iter, long &length, long ct_len, long pd_len)
{
  if (ct_len == 0 && pd_len == 0)
    return ;

  auto len1 = ct_len > length ? length : ct_len;                   // judge if we have enough space to deal with

  transform_data_.insert(transform_data_.cend(), iter, iter + len1);
  iter += len1;
  length -= len1;
  ct_len -= len1;

  if (length == 0) {
    transform_len_ += ct_len;
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

/**
 * @brief To parse the EndRequestRecord part
 * @param iter Buffer's iterator
 */
void ResponseParser::ParseEndRequest(const_iter &iter)
{
  iter -= sizeof(Header);                               // back to the Header's start to constrcut the EndRequestRecord

  EndRequestRocord end_request_record(iter);

  app_status_ = end_request_record.app_status();
  status_ = end_request_record.protocol_status();

  complete_ = true;
  state_ = State::COMPLETED;
}

void ResponseParser::Reset()
{
  request_id_ = -1;
  transform_len_ = 0;
  padding_len_ = 0;
  app_status_ = -1;
  complete_ = false;
  in_content_ = false;
  state_ = State::UNCOMPLETED;
  status_ = Status::FCGI_UNKNOWN_ROLE;

  transform_data_.clear();
  name_value_data_.clear();
}
