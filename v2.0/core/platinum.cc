/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/platinum.h"

#include <thread>

#include "reactor/event_loop.h"
#include "net/tcp_server.h"
#include "config/config.h"

using namespace platinum;

Platinum::Platinum(int num)
  : thread_num_(num > std::thread::hardware_concurrency()
                    ? std::thread::hardware_concurrency()
                    : num)
{ ; }

Platinum::~Platinum()
{
  for (auto &var : threads_)
    var.join();
}

void Platinum::Start()
{
  const auto &config = platinum::Config::GetInstance();
  for (int i = 0; i < thread_num_; ++i) {
    platinum::EventLoop loop;
    platinum::IPAddress addr(config.port());
    platinum::TcpServer server(&loop, addr);
    server.SetMessageCallback();
    server.Start();
  }
}
