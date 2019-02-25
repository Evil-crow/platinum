/**
 * Created by Crow on 12/29/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This is Http Parser base header,
 *         mianly defines states for FSM
 */

#ifndef PLATINUM_HTTP_BASE_H
#define PLATINUM_HTTP_BASE_H

#include <vector>

namespace platinum {
namespace http {
using HttpData = unsigned char;

enum Method : int {
  HTTP_GET,
  HTTP_HEAD,
  HTTP_POET,
  HTTP_UNKNOWN,
};

enum HttpState : int {
  HTTP_STATUS_LINE,
  HTTP_HEADERS,
  HTTP_CR,
  HTTP_LF,
  HTTP_COMPLETE,
  HTTP_FAULT,
};

enum LineState : int {
  LINE_METHOD,
  LINE_SPACE_FIRST,
  LINE_URL,
  LINE_SPACE_SECOND,
  LINE_VERSION_H,
  LINE_VERSION_T_1,
  LINE_VERSION_T_2,
  LINE_VERSION_P,
  LINE_VERSION_SLASH,
  LINE_VERSION_MAJOR,
  LINE_VERSION_DOT,
  LINE_VERSION_MINOR,
  LINE_CR,
  LINE_LF,
  LINE_COMPLETE,
  LINE_FAULT,
};

enum HeaderState : int {
  HEADER_KEY,
  HEADER_COLON,
  HEADER_SPACE,
  HEADER_VALUE,
  HEADER_CR,
  HEADER_LF,
  HEADER_COMPLETE,
  HEADER_FAULT,
};

}
}

#endif //PLATINUM_HTTP_BASE_H
