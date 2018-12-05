#include <iostream>
#include <cstdio>
#include <memory>
#include <arpa/inet.h>

#include "net/socket.h"
#include "net/ip_address.h"
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
//    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
//    sockaddr_in addr{};
//    addr.sin_family = AF_INET;
//    addr.sin_port =::htons(9877);
//    addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//
//    int val = 1;
//    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
//    ::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr));
//    ::listen(fd, SOMAXCONN);

    // IPAddress + Socket
    IPAddress addr(9877);
    Socket acceptor;
    acceptor.SetTcpNoDelay(true);
    acceptor.SetReusePort(true);
    acceptor.Bind(addr);
    acceptor.Listen();

    EventLoop loop;
    auto channel = std::make_shared<Channel>(&loop, acceptor.fd());
//    auto channel = new Channel(&loop, acceptor.fd());
    channel->EnableReading();
    channel->EnableET();
    channel->SetReadCallback([]() { std::cout << "I have callback readable event" << std::endl; });

    loop.AddChannel(channel.get());
    loop.Loop();

    return 0;
}