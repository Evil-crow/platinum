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
  std::vector<std::string> method_list;
};

struct ThreadPool {
  bool thread_pool_enable{};
  int thread_num{};
};

struct FCGI {
  std::string ipaddress;
  std::string unixaddress;
};

struct Resource {
  std::vector<std::string> static_resource;
  std::vector<std::string> dynamic_resource;
  std::vector<std::string> forbidden_resource;
  std::string www_root;
  std::string default_root;
  std::string index;
};

// template specification to adapt User defined type convert
namespace YAML {
template <>
struct convert<Server> {
  static Node encode(const Server &rhs) {
    YAML::Node node;
    node["port"] = rhs.port;
    node["epoll_event"] = rhs.epoll_event;
    node["method"] = rhs.method_list;

    return node;
  }

  static bool decode(const Node &node, Server &rhs) {
    if (!node.IsMap() || node.size() != 3)
      return false;
    rhs.port = node["port"].as<in_port_t>();
    rhs.epoll_event = node["epoll_event"].as<int>();
    rhs.method_list = node["method"].as<std::vector<std::string>>();

    return true;
  }
};

template <>
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

template <>
struct convert<FCGI> {
  static Node encode(const FCGI &rhs) {
    YAML::Node node;
    node["inet"] = rhs.ipaddress;
    node["unix"] = rhs.unixaddress;

    return node;
  }

  static bool decode(const Node &node, FCGI &rhs) {
    if (!node.IsMap() || node.size() != 2)
      return false;
    if (node["inet"].Type() == NodeType::Null && node["unix"].Type() == NodeType::Null) {
      rhs.ipaddress = "NULL";
      rhs.unixaddress = "NULL";
    } else if (node["inet"].Type() == NodeType::Null) {
      rhs.ipaddress = "NULL";
      rhs.unixaddress = node["unix"].as<std::string>();
    } else if (node["unix"].Type() == NodeType::Null) {
      rhs.ipaddress = node["inet"].as<std::string>();
      rhs.unixaddress = "NULL";
    } else {
      rhs.ipaddress = node["inet"].as<std::string>();
      rhs.unixaddress = node["unix"].as<std::string>();
    }

    return true;
  }
};

template <>
struct convert<Resource> {
  static Node encode(const Resource &rhs) {
    YAML::Node node;
    node["static"] = rhs.static_resource;
    node["dynamic"] = rhs.dynamic_resource;
    node["forbidden"] = rhs.forbidden_resource;
    node["www-root"] = rhs.www_root;
    node["defult-root"] = rhs.default_root;
    node["index"] = rhs.index;

    return node;
  }

  static bool decode(const Node &node, Resource &rhs) {
    if (!node.IsMap() || node.size() != 6)
      return false;
    rhs.static_resource = node["static"].as<std::vector<std::string>>();
    rhs.dynamic_resource = node["dynamic"].as<std::vector<std::string>>();
    rhs.forbidden_resource = node["forbidden"].as<std::vector<std::string>>();
    rhs.www_root = node["www-root"].as<std::string>();
    rhs.default_root = node["default-root"].as<std::string>();
    rhs.index = node["index"].as<std::string>();

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

  auto fcgi_config = config["fcgi"].as<FCGI>();
  auto server_config = config["server"].as<Server>();
  auto resource_config = config["resource"].as<Resource>();
  auto thread_pool_config = config["thread pool"].as<ThreadPool>();

  YAMLData data{};

  data.port = server_config.port;
  data.epoll_events = server_config.epoll_event;

  data.thread_pool_enable = thread_pool_config.thread_pool_enable;
  data.thread_num = thread_pool_config.thread_num;

  data.log_enable = config["log_enable"].as<bool>();
  data.index = resource_config.index;
  data.www_root = resource_config.www_root;
  data.default_root = resource_config.default_root;

  data.method_list = std::set<std::string>(server_config.method_list.cbegin(), server_config.method_list.cend());
  data.static_resource = std::set<std::string>(resource_config.static_resource.cbegin(), resource_config.static_resource.cend());
  data.dynamic_resource = std::set<std::string>(resource_config.dynamic_resource.cbegin(), resource_config.dynamic_resource.cend());
  data.forbidden_resource = std::set<std::string>(resource_config.forbidden_resource.cbegin(), resource_config.forbidden_resource.cend());


  try {
    if (fcgi_config.ipaddress == "NULL" &&
        fcgi_config.unixaddress != "NULL") {
      data.fcgi_unixaddress = fcgi_config.unixaddress;
    } else if (fcgi_config.unixaddress == "NULL" &&
        fcgi_config.ipaddress != "NULL") {
      auto port_s = std::string(fcgi_config.ipaddress, fcgi_config.ipaddress.find(':') + 1);
      data.fcgi_inet_ip = std::string(fcgi_config.ipaddress, 0, fcgi_config.ipaddress.find(':'));
      data.fcgi_inet_port = static_cast<in_port_t>(std::strtol(port_s.c_str(), nullptr, 0));
    } else {
      throw std::runtime_error("Using both or neither communication modes at the same time");
    }
  } catch (std::runtime_error &e) {
    std::fprintf(stderr, "%s", e.what());
    std::abort();
  }

  return data;
}
