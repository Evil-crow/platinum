/*
 * @filename:    reactor_test.cc
 * @author:      Crow
 * @date:        12/04/2018 22:47:51
 * @description:
 */

#include <iostream>
#include <cstdio>
#include "../reactor/channel.h"
#include "../reactor/epoller.h"
#include "../reactor/event_loop.h"
#include "../net/socket.h"
#include "../net/ip_address.h"

using namespace platinum;

void func()
{
    std::cout << "I have callback readable event" << std::endl;
}


int main(void)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port =::htons(9877);
    addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

    ::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr));
    ::listen(fd, SOMAXCONN);

    EventLoop loop;
    auto channel = new Channel(&loop, fd);
    channel->EnableReading();
    channel->EnableET();
    channel->SetReadCallback([]() { func(); });

    loop.AddChannel(channel);
    loop.Loop();

    return 0;
}
