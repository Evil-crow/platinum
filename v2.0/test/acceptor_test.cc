//
// Created by Crow on 12/10/18.
//

#define CATCH_CONFIG_MAIN

#include <iostream>
#include <catch2/catch.hpp>

#include "reactor/event_loop.h"
#include "net/acceptor.h"

void func(int fd, const platinum::IPAddress &addr)
{
  std::cout << "fd: " << fd << std::endl;
  std::cout << "ip: " << addr.ip() << std::endl;
  std::cout << "port: " << addr.port() << std::endl;
}

TEST_CASE("Acceptor", "[single-file]")
{
  platinum::IPAddress addr(9877);
  platinum::EventLoop loop;
  platinum::Acceptor acceptor(&loop, addr);
  acceptor.Listening();
  acceptor.SetConnectionCallback(func);
  loop.Loop();
}