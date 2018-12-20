//
// Created by Crow on 12/17/18.
//

#ifndef PLATINUM_WRITEQUEUE_H
#define PLATINUM_WRITEQUEUE_H

#include <deque>
#include <memory>
#include <functional>

namespace platinum {

class WriteTask;
class WriteQueue {
 public:
  void TaskInQueue(int fd, const void *data, size_t total);
//  void TaskInQueue(int outfd, int infd, size_t total);
  bool DoTask();
 private:
  // in order to avoid STL memory leak => shared_ptr, NOT unique_ptr ? think the essence of STL behavior
  std::deque<WriteTask *> queue_;
};

}

#endif //PLATINUM_WRITEQUEUE_H
