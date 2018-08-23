//
// Created by Crow on 8/21/18.
//

#include <thread>
#include <vector>
#include <functional>
#include <unistd.h>
#include "thread_pool.h"

PlatinumServer::thread_pool::thread_pool(int num)
        : flag(false), thread_num(num >= thread_pool::thread_max_num() ? thread_max_num() : num)
{
    for (int i = 0; i < num; ++i)
        thread_vector.emplace_back(std::thread(std::mem_fn(&thread_pool::execute), this));
}

PlatinumServer::thread_pool::~thread_pool()
{
    this->release();
}

bool PlatinumServer::thread_pool::submit_task(thread_pool::task &&task_data)
{
    task_queue.push(std::move(task_data));

    return true;
}

void PlatinumServer::thread_pool::execute()
{
    while (!flag.load()) {
        thread_pool::task func;
        if (!task_queue.pop(func))
            continue;
        else {
            func();
        }
    }
}

void PlatinumServer::thread_pool::release()
{
    flag.store(true);
    sleep(0);
    for (auto &i : thread_vector)
        i.join();
    thread_vector.clear();
}


