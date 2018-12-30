/* 
 * Created by Crow on 12/22/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief:  This file contains all the pre-defination varible
 *         FastCGI offical use #define,
 *         I suppose to use enum class (C++11) to description the variables
 */

#ifndef PLATINUM_BASE_H
#define PLATINUM_BASE_H

namespace platinum {
namespace fcgi {

using FCGIData = unsigned char;
/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"

enum Base : int {
  /*
   * Number of bytes in a FCGI_Header.  Future versions of the protocol
   * will not reduce this number.
   */
      FCGI_HEADER_LEN = 8,

  /*
   * Value for version component of FCGI_Header
   */
      FCGI_VERSION_1 = 1,

  /*
   * Value for requestId component of FCGI_Header
   */
      FCGI_NULL_REQUEST_ID = 0,

  /*
   * Mask for flags component of FCGI_BeginRequestBody
   */
      FCGI_KEEP_CONN = 1,

  /*
   * Send STDIN data max length each time
   */
      FCGI_MAX_LENGTH = 1024,
};

enum Role : int {
  /*
   * Values for role component of FCGI_BeginRequestBody
   */
      FCGI_RESPONDER = 1,
  FCGI_AUTHORIZER = 2,
  FCGI_FILTER = 3,
};

enum Status : int {
  /*
   * Values for protocolStatus component of FCGI_EndRequestBody
   */
      FCGI_REQUEST_COMPLETE = 0,
  FCGI_CANT_MPX_CONN = 1,
  FCGI_OVERLOADED = 2,
  FCGI_UNKNOWN_ROLE = 3,
};

enum Type : int {
  /*
   * Values for type component of FCGI_Header
   */
  FCGI_BEGIN_REQUEST = 1,
  FCGI_ABORT_REQUEST = 2,
  FCGI_END_REQUEST = 3,
  FCGI_PARAMS = 4,
  FCGI_STDIN = 5,
  FCGI_STDOUT = 6,
  FCGI_STDERR = 7,
  FCGI_DATA = 8,
  FCGI_GET_VALUES = 9,
  FCGI_GET_VALUES_RESULT = 10,
  FCGI_UNKNOWN_TYPE = 11,
  FCGI_MAXTYPE = 11,
};

}
}

#endif //PLATINUM_BASE_H
