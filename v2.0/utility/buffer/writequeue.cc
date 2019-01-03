/**
 * Created by Crow on 12/17/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente WriteQueue,
 * @update Add Task later to use other write syscall
 */

#include "writequeue.h"

#include <memory>
#include <iostream>

#include "utility/buffer/task.h"

using namespace platinum;

// Because IO event belongs to a specifical thread,
// so this won't cause race condition,
// This means , we need't use lock to make it thread-safe

void WriteQueue::TaskInQueue(int fd, const char *data, off64_t completed, size_t total)
{
  queue_.push_front(std::make_shared<WriteTask>(fd, data, completed, total));
//  queue_.push_front(std::shared_ptr<Task>(new WriteTask(fd, data, total)));
}

void WriteQueue::TasKInQueue(int fd, const std::string &data, off64_t completed)
{
  TaskInQueue(fd, data.c_str(), completed, data.size());
}

void WriteQueue::TaskInQueue(int outfd, int infd, off64_t completed, size_t total)
{
  queue_.push_front(std::make_shared<SendTask>(outfd, infd, completed, total));
//  queue_.push_front(std::shared_ptr<Task>(new SendTask(outfd, infd, total)));
}


/*
 * prototype:   bool WriteQueue::DoTask()
 * description: get the task and do it, then return result for HandleWrite
 * tips:        get the first task, this won't fail, because if the queue is empty,
 *              we won't trigger WRITEABLE Event
 */
bool WriteQueue::DoTask()
{
  while (!queue_.empty()) {             // if we complete all the task, we should get out.
    auto var = queue_.front();
    if (var->operator()()) {            // complete the task
      queue_.pop_front();
    } else {                            // non-complete the task, e.g. EAGAIN / EWOULDBLOCK
      break;
    }
  }
  return queue_.empty();
}