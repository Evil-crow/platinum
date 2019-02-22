/**
 * Created by Crow on 12/17/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente WriteQueue,
 * @update Add Task later to use other write syscall
 */

#include "writequeue.h"

#include <memory>

#include "utility/buffer/task.h"

using namespace platinum;

// Because IO event belongs to a specifical thread,
// so this won't cause race condition,
// This means , we need't use lock to make it thread-safe

void WriteQueue::TaskInQueue(int fd, const unsigned char *data, off64_t completed, size_t total)
{
  queue_.emplace_back(std::make_shared<WriteTask>(fd, data, completed, total));
}

void WriteQueue::TasKInQueue(int fd, const std::string &data, off64_t completed)
{
  TaskInQueue(fd, data.c_str(), completed, data.size());
}

void WriteQueue::TaskInQueue(int outfd, const std::string &pathname, off64_t completed, size_t total)
{
  queue_.emplace_back(std::make_shared<SendTask>(outfd, pathname, completed, total));
}


/*
 * prototype:   bool WriteQueue::DoTask()
 * description: get the task and do it, then return result for HandleWrite
 * tips:        get the first task, this won't fail, because if the queue is empty,
 *              we won't trigger WRITEABLE Event
 */
bool WriteQueue::DoTask()
{
  while (!queue_.empty()) {                        // if we complete all the task, we should get out.
    if (queue_.front()->operator()()) {            // complete the task, std::queue::front() => reference, this can change the state of the elements in the container.
      queue_.pop_front();
    } else {                            // non-complete the task, e.g. EAGAIN / EWOULDBLOCK
      break;
    }
  }
  return queue_.empty();
}
