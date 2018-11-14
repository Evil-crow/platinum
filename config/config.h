//
// Created by Crow on 11/13/18.
//

#ifndef PLATINUM_CONFIG_H
#define PLATINUM_CONFIG_H

#include <netinet/in.h>
#include <string>
#include <set>

namespace Platinum {

struct YAMLData {
  int epoll_events;
  in_port_t server_port;

  std::string www_root;
  std::string index;

  bool log_enable;
  std::string log_path;

  bool thread_pool_enable;
  int thread_num;

  std::set<std::string> method_list;
};

class Config {
 public:
  explicit Config(YAMLData &&data) noexcept : data_(data) { ; }
//  ~Config() = default;
//  Config(const Config &) = delete;
//  Config operator=(const Config &) = delete;
//  Config(const Config &&) = delete;
//  Config operator=(Config &&) = delete;

  int epoll_event() const { return data_.epoll_events; }
  in_port_t server_port() const { return data_.server_port; }

  std::string www_root() const { return data_.www_root; }
  std::string index() const { return data_.index; }

  bool IsThreadPool() const { return data_.thread_pool_enable;}
  int thread_num() const { return data_.thread_num; }

  bool IsGetVaild() const { return data_.method_list.count("GET") > 0; }
  bool IsPostvaild() const { return data_.method_list.count("HEAD") > 0; }
  bool IsHeadVaild() const { return data_.method_list.count("HEAD") > 0; }
  bool IsOptionVaild() const { return data_.method_list.count("OPTION") > 0; }

  static YAMLData GetData();
 private:
  YAMLData data_;
};

}

#endif //PLATINUM_CONFIG_H
