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
  param_map.insert({"REMOTE_PORT", "80"});
  param_map.insert({"REMOTE_ADDR", "127.0.0.1"});
  param_map.insert({"REQUEST_METHOD", "POST"});
  param_map.insert({"SERVER_PROTOCOL", "HTTP/1.1"});
  param_map.insert({"SCRIPT_FILENAME", "/home/Crow/1.php"});
  param_map.insert({"CONTENT_LENGTH", "11"});
  std::string in_str("a=b&c=d&e=f");
  platinum::fcgi::RequestBuilder builder(3, 11, in_str, param_map);

  builder.Build();

  auto b = builder.begin_requset();
  auto p = builder.fcgi_params();
  auto i = builder.fcgi_in();

  errno = 0;
  ssize_t ret{};
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(9000);
  addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
  ::connect(fd, (const struct sockaddr *) &addr, sizeof(sockaddr_in));
  if (ret < 0)
    perror("connect");

  ::write(fd, reinterpret_cast<void *>(&b), sizeof(b));
  for (const auto &var : p) {
    ::write(fd, reinterpret_cast<void *>(var.first.get()), static_cast<size_t>(var.second));
    perror("write");
  }
  for (const auto &var : i) {
    ::write(fd, reinterpret_cast<void *>(var.first.get()), static_cast<size_t>(var.second));
    perror("write");
  }

  char buf[10000];
//    int k = ::read(fd, buf, 10000);
  char *ptr = buf;
  std::vector<unsigned char> data(1024);
  platinum::fcgi::ResponseParser parser;
  while (!parser.Complete()) {
    ret = ::read(fd, data.data(), 1024);
    bool res = parser.feed(data.cbegin(), static_cast<int>(ret));
    auto stdout_ = parser.transform_data();
    std::string str(stdout_.cbegin(), stdout_.cend());
    std::cout << str << std::endl;
    auto id = parser.request_id();
    auto app_status = parser.app_status();
    auto status = parser.status();
  }
  close(fd);

  return 0;
}
