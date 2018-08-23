#include <iostream>
#include <thread>
#include <unistd.h>
#include "thread_pool.h"

void foo()
{
    std::cout << "this is foo" << std::endl;
}

void too()
{
    std::cout << "this is too" << std::endl;
}

void func()
{
    std::cout << "this is func" << std::endl;
}

void fuck()
{
    std::cout << "this is fuck" << std::endl;
}

int main(int argc, char *argv[])
{
    PlatinumServer::thread_pool pool(2);

    auto i = [](){ return foo(); };
    auto j = [](){ return too(); };
    auto k = [](){ return func(); };
    auto l = [](){ return fuck(); };

    pool.submit_task({i});
    pool.submit_task({j});
    pool.submit_task({k});
    pool.submit_task({l});
    usleep(100);

    return 0;
}