/**
 * Created by Crow on 12/17/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe WriteQueue
 */

#ifndef PLATINUM_TASK_H
#define PLATINUM_TASK_H

#include <cstdio>
namespace platinum {

class WriteQueue;
class Task {
 public:
//  Task() = default;
  Task(int fd, off64_t completed, size_t total);
  virtual ~Task() = default;
  virtual bool operator()() = 0;

 protected:
  int fd_{};
  size_t total_{};
  size_t remained_{};
  off_t completed_{};
};

class WriteTask : public Task {
 public:
  WriteTask(int fd, const char *data, off64_t completed, size_t total);
  ~WriteTask() override = default;
  bool operator()() final;

 private:
  const char *data_;
};

class SendTask : public Task {
 public:
  SendTask(int outfd, int infd, off64_t completed, size_t total);
  ~SendTask() override = default;
  bool operator()() final;

 private:
  int infd_;
};

}
#endif //PLATINUM_TASK_H
