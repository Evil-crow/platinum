//
// Created by Crow on 8/21/18.
//

#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <unistd.h>
#include "threadpool/thread_pool.h"

thread_pool::thread_pool(int num) : flag(false),
        thread_num(num >= thread_pool::thread_max_num() ? thread_max_num() : num)
{
    for (int i = 0; i < num; ++i)
        thread_vector.emplace_back(std::thread(std::mem_fn(&thread_pool::execute), this));
}

bool thread_pool::submit_task(thread_pool::task &&task_data)
{
    task_queue.push(std::move(task_data));

    return true;
}

void thread_pool::execute()
{
    while (!flag.load()) {
        thread_pool::task func;
        if (!task_queue.pop(func))
            continue;
        else {
            func();
            std::cout << "get task" << std::endl;
        }
    }
}

void thread_pool::release()
{
    flag.store(true);
    sleep(0);
    this->stop();
}

void thread_pool::stop()
{
    for (auto &i : thread_vector)
        i.join();
    thread_vector.clear();
}


