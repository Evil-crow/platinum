/* 
 * Created by Crow on 12/22/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * brief: 
 */

#include "component.h"

#include "base.h"

using namespace fcgi;

Header::Header(Type type,
               int request_id,
               int content_length,
               int padding_length)
    : version_(Base::FCGI_VERSION_1),
      type_(type),
      request_idB1(static_cast<unsigned char>((request_id >> 8)     & 0xff)),
      request_idB0(static_cast<unsigned char>(request_id            & 0xff)),
      content_lengthB1(static_cast<unsigned char>((content_length >> 8) & 0xff)),
      content_lengthB0(static_cast<unsigned char>(content_length        & 0xff)),
      padding_length_(static_cast<unsigned char>(padding_length)),
      reserved_(0)
{
  ;
}

BeginRequestRecord::BeginRequestRecord(fcgi::Role role_,
                                       bool keeplive,
                                       fcgi::Type type,
                                       int request_id,
                                       int content_length,
                                       int pending_length)
    : header_(type, request_id, content_length, pending_length),
      roleB1(static_cast<unsigned char>((role_ >> 8) & 0xff)),
      roleB0(static_cast<unsigned char>(role_        & 0xff)),
      flags_(static_cast<unsigned char>(keeplive ? 1 : 0))
{
  ;
}