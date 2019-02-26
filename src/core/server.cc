/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/server.h"

#include <csignal>
#include <thread>

#include "core/affair.h"
#include "include/net.hpp"
#include "include/config.hpp"

using namespace platinum;

void Server::Exec()
{
  for (auto &var : threads_)
    var.join();
}

void Server::Start()
{
  const auto &config = platinum::Config::GetInstance();

  // ensure that there are not too many threads
  thread_num_ = config.thread_num() > std::thread::hardware_concurrency()
                                    ? std::thread::hardware_concurrency()
                                    : config.thread_num();

  // Start server::loop in threads
  for (int i = 0; i < thread_num_; ++i) {
    threads_.emplace_back([&config](){
      platinum::EventLoop loop;
      platinum::IPAddress addr(config.port());
      platinum::TcpServer instance(&loop, addr);
      instance.SetMessageCallback(platinum::func);
      instance.Start();
    });
  }
}
