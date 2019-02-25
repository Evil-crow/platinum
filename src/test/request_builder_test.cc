/**
 * Created by Crow on 12/27/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to test Class RequestBuilder
 * @details Use map to store params, then RequestBuilder::Build().
 *          send data to php-fpm, can get the result by tcpdump [if php-fpm listened on TCP socket]
 *          $ tcpdump port 9000 -i lo -vv -w a.cap
 *          $ wireshark a.cap
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include "protocol/fastCGI/request_builder.h"

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


  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(9000);
  addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
  int ret = ::connect(fd, (const struct sockaddr *) &addr, sizeof(sockaddr_in));
  if (ret < 0)
    perror("connect");

  ::write(fd, reinterpret_cast<void *>(&b), sizeof(b));
  for (const auto &var : p)
    ::write(fd, reinterpret_cast<void *>(var.first.get()), var.second);

  for (const auto &var : i)
    ::write(fd, reinterpret_cast<void *>(var.first.get()), var.second);

  close(fd);

  return 0;
}
