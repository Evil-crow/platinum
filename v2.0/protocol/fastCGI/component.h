/* 
 * Created by Crow on 12/22/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief: 
 */

#ifndef PLATINUM_COMPONENT_H
#define PLATINUM_COMPONENT_H

#include "base.h"

namespace fcgi {

class Header {
 public:
  Header(Type type, int request_id, int content_length, int pending_length);
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

struct EndRequestRocord {
  Header header_;
  unsigned char appStatusB3;
  unsigned char appStatusB2;
  unsigned char appStatusB1;
  unsigned char appStatusB0;
  unsigned char protocolStatus;
  unsigned char reserved[3];
};

}

#endif //PLATINUM_COMPONENT_H
