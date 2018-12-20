//
// Created by Crow on 12/17/18.
//

#include "writequeue.h"

#include <memory>
#include <iostream>

#include "utility/buffer/task.h"

using namespace platinum;

// Because IO event belongs to a specifical thread,
// so this won't cause race condition,
// This means , we need't use lock to make it thread-safe

void WriteQueue::TaskInQueue(int fd, const void *data, size_t total)
{
  std::cout << "into taskinqueue" << '\n';
  auto ptr = new platinum::WriteTask(fd, data, total);
  queue_.push_front(ptr);
}

//void WriteQueue::TaskInQueue(int outfd, int infd, size_t total)
//{
//  queue_.push_front(new SendTask(outfd, infd, total));
//}

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