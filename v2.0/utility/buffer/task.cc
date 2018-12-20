//
// Created by Crow on 12/21/18.
//

#include "task.h"

#include <unistd.h>
#include <sys/sendfile.h>
#include <cerrno>

#include "utility/logger.h"
using namespace platinum;

Task::Task(int fd, size_t total)
    : fd_(fd),
      total_(total),
      remained_(total),
      completed_(0)
{
  ;
}

WriteTask::WriteTask(int fd, const void *data, size_t total)
    : Task(fd, total),
      data_(data)
{
  ;
}

bool WriteTask::operator()()
{
  while (true) {
    auto var = ::write(fd_, data_ + completed_, remained_);
    if (var == remained_) {
      return true;
    } else if ( var >= 0 && var < remained_) {
      remained_ -= var;
      completed_ += var;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
      } else {
        LOG(ERROR) << "WriteTask::operator()";
      }
    }
  }
}

SendTask::SendTask(int outfd, int infd, size_t total)
    : Task(outfd, total),
      infd_(infd)
{
  ;
}

bool SendTask::operator()()
{
  while (true) {
    auto var = ::sendfile64(fd_, infd_, &completed_, total_);
    if (var == total_) {
      return true;
    } else if (var >= 0 && var < remained_) {
      remained_ -= var;
      completed_ += var;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
      } else {
        LOG(ERROR) << "SendTask::operator()";
      }
    }
  }
}