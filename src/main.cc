#include <iostream>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "threadpool/thread_pool.h"

void foo(void)
{
    std::cout << "this is foo" << std::endl;
}

void too(void)
{
    std::cout << "this is too" << std::endl;
}

void func(void)
{
    std::cout << "this is func" << std::endl;
}

void fuck(void)
{
    std::cout << "this is fuck" << std::endl;
}

int main(int argc, char *argv[])
{
    thread_pool pool(1);

    auto i = [](){ return foo(); };
    auto j = [](){ return too(); };
    auto k = [](){ return func(); };
    auto l = [](){ return fuck(); };

    pool.submit_task({i});
    pool.submit_task({j});
    pool.submit_task({k});
    pool.submit_task({l});

    sleep(1);
    pool.release();
    return 0;
}