/**
 * Created by Crow on 11/13/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  To load YAML Configuration by yaml-cpp
 */

#include "config.h"

#include <cassert>
#include <string>
#include <utility>

#include <yaml-cpp/yaml.h>

using namespace platinum;

struct Server {
  in_port_t port{};
  int epoll_event{};
};

struct ThreadPool {
  bool thread_pool_enable{};
  int thread_num{};
};

// TODO (Crow, evilcrow486@gmail.com) To add CGI support and its template specification

// template specification to adapt User defined type convert
namespace YAML {
template<>
struct convert<Server> {
  static Node encode(const Server &rhs) {
    YAML::Node node;
    node["port"] = rhs.port;
    node["epoll_event"] = rhs.epoll_event;

    return node;
  }

  static bool decode(const Node &node, Server &rhs) {
    if (!node.IsMap() || node.size() != 2)
      return false;

    rhs.port = node["port"].as<in_port_t>();
    rhs.epoll_event = node["epoll_event"].as<int>();
    return true;
  }
};

template<>
struct convert<ThreadPool> {
  static Node encode(const ThreadPool &rhs) {
    YAML::Node node;
    node["enable"] = rhs.thread_pool_enable;
    node["thread_num"] = rhs.thread_num;

    return node;
  }

  static bool decode(const Node &node, ThreadPool &rhs) {
    if (!node.IsMap() || node.size() != 2)
      return false;
    rhs.thread_pool_enable = node["enable"].as<bool>();
    rhs.thread_num = node["thread_num"].as<int>();

    return true;
  }

};

}

Config &Config::GetInstance()
{
  static Config config(std::move(GetData()));
  return config;
}

YAMLData Config::GetData()
{
  auto config = YAML::LoadFile("../config.yaml");
  assert(config["server"]);
  assert(config["method"]);

  auto server_config = config["server"].as<Server>();
  auto thread_pool_config = config["thread pool"].as<ThreadPool>();
  auto method_seq = config["method"].as<std::vector<std::string>>();
  YAMLData data{};

  data.port = server_config.port;
  data.epoll_events = server_config.epoll_event;

  data.thread_pool_enable = thread_pool_config.thread_pool_enable;
  data.thread_num = thread_pool_config.thread_num;

  data.log_enable = config["log_enable"].as<bool>();
  data.index = config["index"].as<std::string>();
  data.www_root = config["www-root"].as<std::string>();

  data.method_list = std::set<std::string>(method_seq.cbegin(), method_seq.cend());

  return data;
}
