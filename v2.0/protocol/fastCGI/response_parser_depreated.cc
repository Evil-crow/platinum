/* 
 * Created by Crow on 12/24/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief: 
 */

#include "response_parser_depreated.h"
#include <cstring>
#include "component.h"

using namespace fcgi;

ResponseParser::ResponseParser()
    : request_id_(-1),
      complete_(false),
      app_status_(0),
      state_(State::FCGI_UNKNOWN_ROLE),
      parser_state_(STDOUT)
{
  ;
}


long ResponseParser::feed(const std::vector<FCGIData> &data)
{
  if (!complete()) {
    NVPairs.clear();
    fcgi_out_.clear();
    fcgi_err_.clear();
  }

  auto iter = data.cbegin();
  while (parser_state_ == ParserState::STDOUT) {
    Header header(iter);

    if (header.type() != Type::FCGI_STDOUT) {
      if (header.type() == Type::FCGI_STDERR)
        goto JUDGE;
      if (header.type() == Type::FCGI_END_REQUEST)
        goto JUDGE;
      return -1;
    }
    iter += sizeof(Header);
    request_id_ = header.request_id();
    auto ct_len = header.content_length();
    auto pd_len = header.padding_length();
    if (ct_len == 0) {
      iter += ct_len + pd_len;
      break;
    }

    ParserStdin(iter, ct_len);
    iter += ct_len + pd_len;
  }

JUDGE:  ;
  Header headerxx(iter);
  if (headerxx.type() == Type::FCGI_STDERR) {
    parser_state_ = ParserState::STDERR;
    while (parser_state_ == ParserState::STDERR) {
      Header header(iter);
      iter += sizeof(Header);

      if (header.type() != Type::FCGI_STDERR) {
        if (header.type() == Type::FCGI_END_REQUEST) {
          parser_state_ = ParserState::ENDREQUEST;
          goto END;
        } else {
          return -1;
        }
      }

      auto ct_len = header.content_length();
      auto pd_len = header.padding_length();
      ParserStderr(iter, ct_len);
      iter += ct_len + pd_len;
    }
  } else if (headerxx.type() == Type::FCGI_END_REQUEST) {
END:  EndRequestRocord end_requset_record(iter);
      iter += sizeof(EndRequestRocord);

      app_status_ = end_requset_record.app_status();
      state_ = end_requset_record.protocol_status();
  }

  complete_ = true;
  return (iter - data.cbegin());
}

void ResponseParser::ParserStdin(const_iter iter, int ct_len)
{
  std::string data(iter, iter + ct_len);
  std::string pattern("\r\n\r\n");
  auto pos = data.find(pattern);            // find the pos which signs the end of headers.
  auto stdout_len = data.size() - (pos + 4);

  std::shared_ptr<FCGIData> fcgi_stdout(
      new FCGIData[stdout_len],
      std::default_delete<FCGIData []>()
  );

  ::memcpy(fcgi_stdout.get(), &data[pos + 4], stdout_len);
  fcgi_out_.emplace_back(fcgi_stdout, stdout_len);
}

void ResponseParser::ParserNVPairs(const_iter iter_begin, const_iter iter_end)
{

}

void ResponseParser::ParserStderr(const_iter iter, int ct_len)
{
  std::string data(iter, iter + ct_len);
  std::shared_ptr<FCGIData> fcgi_stderr(
      new FCGIData[ct_len],
      std::default_delete<FCGIData []>()
  );

  ::memcpy(fcgi_stderr.get(), &data[0], ct_len);
  fcgi_err_.emplace_back(fcgi_stderr, ct_len);
}

bool ResponseParser::complete()
{
  return complete_;
}
