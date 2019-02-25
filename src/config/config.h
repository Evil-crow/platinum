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

#include <cstring>
#include <set>
#include <string>
#include <vector>
#include <utility>

namespace platinum {

struct YAMLData {
  in_port_t port;

  bool log_enable;

  std::string index;
  std::string www_root;
  std::string default_root;
  std::string fcgi_root;

  std::string fcgi_listen_sock;
  std::pair<std::string, in_port_t> fcgi_inet_addr;
  std::string fcgi_unix_addr;

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

  in_port_t port() const          { return data_.port; }

  bool IsLogOn() const            { return data_.log_enable; }

  std::string www_root() const    { return data_.www_root; }
  std::string default_root() const { return data_.default_root; }
  std::string index() const       { return data_.index; }

  std::string fcgi_root() const   { return data_.fcgi_root; }
  std::string fcgi_unix() const   { return data_.fcgi_unix_addr; }
  std::string fcgi_inet_ip() const { return data_.fcgi_inet_addr.first; }
  in_port_t fcgi_inet_port() const { return data_.fcgi_inet_addr.second; }


  bool IsThreadPoolOn() const     { return data_.thread_pool_enable; }
  bool IsInetAddr() const         { return data_.fcgi_listen_sock == "inet"; }
  bool IsUnixAddr() const         { return data_.fcgi_listen_sock == "unix"; }
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
