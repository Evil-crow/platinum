#include <iostream>
#include "server.h"

#include <vector>
#include <thread>

int main(int argc, char *argv[])
{
    std::vector<std::thread> threads;

    for (int i = 0; i < std::thread::hardware_concurrency() - 1; i++) {
        threads.emplace_back([] {
          epoll_server(8090);
        });
    }

    epoll_server(8090);
    return 0;
}