#include <iostream>

#include "utility/log.hpp"
#include "config/config.h"

using namespace Platinum;

int main(int argc, char *argv[])
{
  Config config(std::move(Config::GetData()));
  std::cout << config.server_port() << std::endl
            << config.epoll_event() << std::endl
            << config.index() << std::endl
            << config.www_root() << std::endl;

  std::cout << "GET: " << config.IsGetVaild() << std::endl;
  std::cout << "POST: " << config.IsHeadVaild() << std::endl;

  LOG("INFO", "Running");
  std::cout << "thread: " << config.IsThreadPool() << std::endl;
  std::cout << "num: " << config.thread_num() << std::endl;

  return 0;
}