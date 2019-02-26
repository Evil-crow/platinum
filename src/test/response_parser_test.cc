/**
 * Created by Crow on 12/27/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is test the Class ResponseParser
 * @details construct the request, send/write it to the peer endpoint.
 *          use tcpdump can get the result [if php-fpm listened on TCP socket]
 *          $ sudo tcpdump port xxxx -i lo -vv -w a.cap
 *          $ wireshark a.cap
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "protocol/fastCGI/request_builder.h"
#include "protocol/fastCGI/response_parser.h"

int main()
{
  std::map<std::string, std::string> param_map;
  std::string in_str("");
  param_map.insert({"REQUEST_METHOD", "GET"});
  param_map.insert({"SCRIPT_FILENAME", "/home/Crow/2.php"});
  param_map.insert({"CONTENT_LENGTH", std::to_string(in_str.size())});
//  param_map.insert({"CONTENT_TYPE", "application/x-www-form-urlencoded"});
  param_map.insert({"QUERY_STRING", "a=20&b=10&c=5&d=6"});
  platinum::fcgi::RequestBuilder builder(3, in_str.size(), in_str, param_map);

  builder.Build();

  auto b = builder.begin_requset();
  auto p = builder.fcgi_params();
  auto i = builder.fcgi_in();
  auto p_e = builder.empty_fcgi_param();
  auto s_i = builder.empty_fcgi_in();

  errno = 0;
  ssize_t ret{};
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(9000);
  addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
  ret = ::connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
  if (ret < 0)
    perror("connect");

  ::write(fd, reinterpret_cast<void *>(&b), sizeof(b));
  for (const auto &var : p)
    ::write(fd, reinterpret_cast<void *>(var.first.get()), static_cast<size_t>(var.second));
  ::write(fd, reinterpret_cast<void *>(p_e.first.get()), static_cast<size_t>(p_e.second));
  for (const auto &var : i)
    ::write(fd, reinterpret_cast<void *>(var.first.get()), static_cast<size_t>(var.second));
  ::write(fd, reinterpret_cast<void *>(s_i.first.get()), static_cast<size_t>(s_i.second));
  char buf[10000];
//    int k = ::read(fd, buf, 10000);
  std::vector<unsigned char> data(1024);
  platinum::fcgi::ResponseParser parser;
  while (!parser.Complete()) {
    ret = ::read(fd, data.data(), 1024);
    parser.Feed(data.cbegin(), static_cast<int>(ret));
    auto stdout_ = parser.transform_data();
    std::string str(stdout_.cbegin(), stdout_.cend());
    std::cout << str << std::endl;
    auto map = parser.key_value();

    for (const auto &var : map)
      std::cout << var.first << " " << var.second << std::endl;
    std::cout << parser.request_id() << std::endl;
    std::cout << parser.app_status() << std::endl;
//    auto status = parser.status();
  }
//  close(fd);

  return 0;
}
