//
// Created by Crow on 12/17/18.
//

#ifndef PLATINUM_WRITEQUEUE_H
#define PLATINUM_WRITEQUEUE_H

#include <deque>
#include <memory>
#include <functional>

namespace platinum {

class Task;
class WriteQueue {
 public:
  void TaskInQueue(int fd, const char *data, off64_t completed, size_t total);
  void TasKInQueue(int fd, const std::string &data, off64_t completed);
  void TaskInQueue(int outfd, int infd, off64_t completed, size_t total);
  bool DoTask();
 private:
  // in order to avoid STL memory leak => shared_ptr, NOT unique_ptr ? think the essence of STL behavior
  std::deque<std::shared_ptr<Task>> queue_;
};

}

#endif //PLATINUM_WRITEQUEUE_H
