/* 
 * Created by Crow on 12/22/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief: This file defines the structure description in FastCGI protocol
 *      such as: Header, BeginRequestRocord, EndRequestRecord
 *      BeginRequestRocord = Header + BeginRequestBody
 *      EndRequestRecord = Header + EndRequestBody
 *      FCGI_Params = Header + N-V pairs
 *      FCGI_Stdin = Header + Data
 *      FCGI_Stdout = Header + Data
 *      FCGI_Stderr = Header + Data
 *
 *      The whole protocol process is:
 *      1. BeginRequestRecord     WS => App
 *      2. FCGI_PARAMS            WS => App
 *         .......(FCGI_PARAMS)
 *      3. Empty FCGI_PARAMS      WS => App  (Sign Params end)
 *      4. FCGI_STDIN             WS => App
 *         .......(FCGI_STDIN)
 *      5. Empty FCGI_STDIN       WS => App  (Sign Stdin end)
 *
 *      At now, Ws => App ends
 *
 *      6. FCGI_STDOUT            WS <= App
 *         ......(FCGI_STDOUT)
 *      7. Empty FCGI_STDOUT      WS <= App
 *     [.] FCGI_STDERR            WS <= App   (If error, or won't send FCGI_STDERR)
 *      8. EndRequestRecord       WS <= App
 *
 *      At now, the whole process ends,
 *      When to cut the connetion,
 *      depends on whether you set the keep-alive in BeginRequestRecord
 */

#ifndef PLATINUM_COMPONENT_H
#define PLATINUM_COMPONENT_H

#include <vector>
#include "base.h"

namespace platinum {
namespace fcgi {

class Header {
 public:
  using const_iter = std::vector<FCGIData>::const_iterator;

  explicit Header(const_iter iter);
  Header(Type type, int request_id, int content_length, int pending_length);

  Type type() { return static_cast<Type>(type_); }
  int request_id() { return (request_idB1 << 8) + request_idB0; }
  int content_length() { return (content_lengthB1 << 8) + content_lengthB0; }
  int padding_length() { return padding_length_; }
 private:
  unsigned char version_;
  unsigned char type_;
  unsigned char request_idB1;
  unsigned char request_idB0;
  unsigned char content_lengthB1;
  unsigned char content_lengthB0;
  unsigned char padding_length_;
  unsigned char reserved_;
};

class BeginRequestRecord {
 public:
  BeginRequestRecord(Role role_,
                     bool keeplive,
                     Type type,
                     int request_id,
                     int content_length,
                     int pending_length);
 private:
  Header header_;
  unsigned char roleB1;
  unsigned char roleB0;
  unsigned char flags_;
  unsigned char reserved_[5];
};

class EndRequestRocord {
 public:
  using const_iter = std::vector<FCGIData>::const_iterator;
  explicit EndRequestRocord(const_iter iter);

  long long app_status() {
    return (appStatusB3 << 24) +
        (appStatusB2 << 16) +
        (appStatusB1 << 8) +
        appStatusB0;
  }

  Status protocol_status() {
    return static_cast<Status>(protocolStatus);
  }

 private:
  Header header_;
  unsigned char appStatusB3;
  unsigned char appStatusB2;
  unsigned char appStatusB1;
  unsigned char appStatusB0;
  unsigned char protocolStatus;
  unsigned char reserved[3];
};

}
}

#endif //PLATINUM_COMPONENT_H
