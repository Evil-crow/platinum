/**
* Created by Crow on 11/13/18.
* Copyright (c) 2018 Crow All rights reserved.
* @author Crow
* @brief  this is for class Config -> to set the configuration for PlatinumServer
*       It's parse YAML file by the library yaml-cpp, You can find it on github
*       And Config mainly to provide an instance of Singleton. You can get it everywhere.
*       And you ask it the configuration is OK
*/

#ifndef PLATINUM_CONFIG_H
#define PLATINUM_CONFIG_H

#include <netinet/in.h>

#include <set>
#include <string>

namespace platinum {

struct YAMLData {
  int epoll_events;
  in_port_t server_port;

  bool log_enable;

  std::string www_root;
  std::string index;

  bool thread_pool_enable;
  int thread_num;

  std::set<std::string> method_list;
};

class Config {
 public:
  static Config &GetInstance();

  Config(const Config &) = delete;
  Config operator=(const Config &) = delete;

  int epoll_event() const         { return data_.epoll_events; }
  in_port_t server_port() const   { return data_.server_port; }

  bool IsLogOn() const            { return data_.log_enable; }

  std::string www_root() const    { return data_.www_root; }
  std::string index() const       { return data_.index; }

  bool IsThreadPoolOn() const     { return data_.thread_pool_enable; }
  int thread_num() const          { return data_.thread_num; }

  bool IsGetVaild() const         { return data_.method_list.count("GET") > 0; }
  bool IsPostvaild() const        { return data_.method_list.count("HEAD") > 0; }
  bool IsHeadVaild() const        { return data_.method_list.count("HEAD") > 0; }
  bool IsOptionVaild() const      { return data_.method_list.count("OPTION") > 0; }

 private:
  explicit Config(YAMLData &&data) : data_(data) { ; }
  static YAMLData GetData();

  YAMLData data_;
};

}

#endif //PLATINUM_CONFIG_H
