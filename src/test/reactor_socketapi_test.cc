#include <iostream>
#include <cstdio>
#include <memory>
#include <arpa/inet.h>

#include "net/socket.h"
#include "net/ip_address.h"
#include "net/acceptor.h"
#include "reactor/event_loop.h"
#include "reactor/channel.h"
#include "reactor/epoller.h"

using namespace platinum;

void func()
{
  std::cout << "I have callback readable event" << std::endl;
}

int main(int argc, char *argv[])
{
//    socket APIs
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port =::htons(9877);
    addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

    int val = 1;
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
    ::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr));
    ::listen(fd, SOMAXCONN);

    EventLoop loop;
    auto channel = new Channel(&loop, fd);
    channel->EnableReading();
    channel->EnableET();
    channel->SetReadCallback([]() { std::cout << "I have callback readable event" << std::endl; });

    loop.AddChannel(channel);
    loop.Loop();

    return 0;
}
