/**
* Created by Crow on 11/13/18.
* Copyright (c) 2018 Crow All rights reserved.
* @author Crow
* @brief  this is for class Config -> to set the configuration for PlatinumServer
*       It's parse YAML file by the library yaml-cpp, You can find it on github
*       And Config mainly to provide an instance of Singleton. You can get it everywhere.
*       And you ask it the configuration is OK
*/

#ifndef PLATINUM_CONFIG_CONFIG_H
#define PLATINUM_CONFIG_CONFIG_H

#include <netinet/in.h>

#include <set>
#include <string>
#include <vector>

namespace platinum {

struct YAMLData {
  int epoll_events;
  in_port_t port;

  bool log_enable;

  std::string index;
  std::string www_root;
  std::string default_root;

  std::string fcgi_inet_ip;
  in_port_t fcgi_inet_port;
  std::string fcgi_unixaddress;

  bool thread_pool_enable;
  int thread_num;

  std::set<std::string> method_list;
  std::set<std::string> static_resource;
  std::set<std::string> dynamic_resource;
  std::set<std::string> forbidden_resource;
};

class Config {
 public:
  static Config &GetInstance();

  Config(const Config &) = delete;
  Config operator=(const Config &) = delete;

  int epoll_event() const         { return data_.epoll_events; }
  in_port_t port() const          { return data_.port; }

  bool IsLogOn() const            { return data_.log_enable; }

  std::string www_root() const    { return data_.www_root; }
  std::string default_root() const { return data_.default_root; }
  std::string index() const       { return data_.index; }

  bool IsThreadPoolOn() const     { return data_.thread_pool_enable; }
  int thread_num() const          { return data_.thread_num; }

  const std::set<std::string> &method_list() const          { return data_.method_list; }
  const std::set<std::string> &static_resource() const      { return data_.static_resource; }
  const std::set<std::string> &dynamic_resource() const     { return data_.dynamic_resource; }
  const std::set<std::string> &forbidden_resource() const   { return data_.forbidden_resource; }

 private:
  explicit Config(YAMLData &&data) : data_(data) { ; }
  static YAMLData GetData();

  YAMLData data_;
};

}

#endif //PLATINUM_CONFIG_CONFIG_H
