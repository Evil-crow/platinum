/**
 * Created by Crow on 12/29/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is Class RequestParser immplments
 */

#include "request_parser.h"

#include <cctype>
//#include "utility/logger.h"

using namespace platinum::http;

RequestParser::RequestParser()
    : http_state_(HttpState::HTTP_STATUS_LINE),
      line_state_(LineState::LINE_METHOD),
      header_state_(HeaderState::HEADER_KEY),
      complete_(false)
{
  ;
}

/**
 * @brief To parse HTTP protocol, can be called many times
 * @param iter iterator of buffer from Net part
 * @param length buffer length.
 * @return -1 means error, or return the length of an request we have parsed.
 */

int RequestParser::feed(const_iter iter, int length)
{
  auto len(length);
  if (http_state_ == HTTP_FAULT) {
    complete_ = true;
//    LOG(ERROR) << "RequestParser::feed() => parse fault";
    return -1;
  }

  switch (http_state_) {
    case HTTP_STATUS_LINE:
      if (ParseStatusLine(iter, length)) {
        http_state_ = HTTP_HEADERS;
      } else {
        break;
      }

    case HTTP_HEADERS:
      if (ParseHeader(iter, length)) {
          http_state_ = HTTP_CR;
      } else {
        break;
      }

    case HTTP_CR:
      if (length) {
        if (*iter == '\r') {
          http_state_ = HTTP_LF;
          length--;
          iter++;
        } else {
          http_state_ = HTTP_FAULT;
          break;
        }
      } else {
        break;
      }

    case HTTP_LF:
      if (length) {
        if (*iter == '\n') {
          http_state_ = HTTP_COMPLETE;
          length--;
          iter++;

          if (HasBody())
            body_len_ = std::strtol(header("Content-Length").c_str(), nullptr, 10);
        } else {
          http_state_ = HTTP_FAULT;
          break;
        }
      } else {
        break;
      }
    case HTTP_COMPLETE:
      if (HasBody() && body_len_) {
        ParseBody(iter, length);
      }
      complete_ = true;

    default: break;
  }

  return (len - length);
}

/**
 * @brief To parse StatusLine part
 * @param iter as above
 * @param length as above
 * @return unless an parse complete, we won't return false.
 */
bool RequestParser::ParseStatusLine(const_iter &iter, int &length)
{
  switch (line_state_) {
    case LINE_METHOD:
      while (length) {
        if (std::isalpha(*iter)) {
          method_ += *iter;
          length--;
          iter++;
        } else if (std::isspace(*iter)) {
          line_state_ = LINE_SPACE_FIRST;
          break;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      }

    case LINE_SPACE_FIRST:
      if (length) {
        if (std::isspace(*iter)) {
          line_state_ = LINE_URL;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
          goto exit;
      }

    case LINE_URL:
      while (length) {
        if (std::isspace(*iter)) {
          line_state_ = LINE_SPACE_SECOND;
          break;
        } else {
          url_ += *iter;
          length--;
          iter++;
        }
      }

    case LINE_SPACE_SECOND:
      if (length) {
        if (std::isspace(*iter)) {
          line_state_ = LINE_VERSION_H;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_H:
      if (length) {
        if (*iter == 'H') {
          line_state_ = LINE_VERSION_T_1;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_T_1:
      if (length) {
        if (*iter == 'T') {
          line_state_ = LINE_VERSION_T_2;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_T_2:
      if (length) {
        if (*iter == 'T') {
          line_state_ = LINE_VERSION_P;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_P:
      if (length) {
        if (*iter == 'P') {
          line_state_ = LINE_VERSION_SLASH;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_SLASH:
      if (length) {
        if (*iter == '/') {
          line_state_ = LINE_VERSION_MAJOR;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_MAJOR:
      if (length) {
        if (std::isdigit(*iter)) {
          line_state_ = LINE_VERSION_DOT;
          version_major_ = (*iter - '0');
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_DOT:
      if (length) {
        if (*iter == '.') {
          line_state_ = LINE_VERSION_MINOR;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_VERSION_MINOR:
      if (length) {
        if (std::isdigit(*iter)) {
          line_state_ = LINE_CR;
          version_minor_ = (*iter - '0');
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_CR:
      if (length) {
        if (*iter == '\r') {
          line_state_ = LINE_LF;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_LF:
      if (length) {
        if (*iter == '\n') {
          line_state_ = LINE_COMPLETE;
          length--;
          iter++;
        } else {
          line_state_ = LINE_FAULT;
          http_state_ = HTTP_FAULT;
          goto exit;
        }
      } else {
        goto exit;
      }

    case LINE_COMPLETE:
      return true;

    default:
      goto exit;
  }

exit:
  return false;
}

bool RequestParser::ParseHeader(const_iter &iter, int &length)
{
  while (length) {
    if (header_state_ == HEADER_COMPLETE) {
      if (*iter == '\r') {
        return true;
      }
      else
        header_state_ = HEADER_KEY;
    }

    switch (header_state_) {
      case HEADER_KEY:
        while (length) {
          if (*iter != ':') {
            key_ += *iter;
            length--;
            iter++;
          } else {
            header_state_ = HEADER_COLON;
            break;
          }
        }

      case HEADER_COLON:
        if (length) {
          if (*iter == ':') {
            header_state_ = HEADER_SPACE;
            length--;
            iter++;
          } else {
            header_state_ = HEADER_FAULT;
            http_state_ = HTTP_FAULT;
            goto exit;
          }
        } else {
          goto exit;
        }

      case HEADER_SPACE:
        if (length) {
          if (std::isspace(*iter)) {
            header_state_ = HEADER_VALUE;
            length--;
            iter++;
          } else {
            header_state_ = HEADER_FAULT;
            http_state_ = HTTP_FAULT;
            goto exit;
          }
        } else {
          goto exit;
        }

      case HEADER_VALUE:
        while (length) {
          if (*iter != '\r') {
            value_ += *iter;
            length--;
            iter++;
          } else {
            header_state_ = HEADER_CR;
            break;
          }
        }

      case HEADER_CR:
        if (length) {
          if (*iter == '\r') {
            header_state_ = HEADER_LF;
            length--;
            iter++;
          } else {
            header_state_ = HEADER_FAULT;
            http_state_ = HTTP_FAULT;
            goto exit;
          }
        } else {
          goto exit;
        }

      case HEADER_LF:
        if (length) {
          if (*iter == '\n') {
            header_state_ = HEADER_COMPLETE;
            length--;
            iter++;
          } else {
            header_state_ = HEADER_FAULT;
            http_state_ = HTTP_FAULT;
            goto exit;
          }
        } else {
          goto exit;
        }

      case HEADER_COMPLETE:
        key_value_map_.insert({key_, value_});
        key_.clear();
        value_.clear();
        break;

      default:
exit:   return false;
    }
  }
}

bool RequestParser::ParseBody(const_iter &iter, int &length)
{
  auto len = body_len_ > length ? length : body_len_;
  body_len_ -= len;
  body_.insert(body_.cend(), iter, iter + len);

  return true;
}

bool RequestParser::Complete() const
{
  return complete_;
}

bool RequestParser::HasBody()
{
  return header("Content-Length") != std::string("");
}
