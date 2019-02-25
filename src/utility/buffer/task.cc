/**
 * Created by Crow on 11/21/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente Task,
 *
 */

#include "task.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <cerrno>
#include <cstring>

#include "utility/logger.h"
using namespace platinum;

Task::Task(int fd, off64_t completed, size_t total)
    : fd_(fd),
      total_(total),
      remained_(total),
      completed_(completed)
{
  ;
}

WriteTask::WriteTask(int fd, const unsigned char *data, off64_t  completed, size_t total)
    : Task(fd, completed, total)
{
  data_ = std::shared_ptr<unsigned char>(new unsigned char[total], std::default_delete<unsigned char []>());
  ::memcpy(data_.get(), data, total);
}

bool WriteTask::operator()()
{
  while (true) {
    auto var = ::write(fd_, data_.get() + completed_, remained_);
    if (var == remained_) {
      return true;
    } else if ( var >= 0 && var < remained_) {
      remained_ -= var;
      completed_ += var;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
      } else {
        perror("write:");
        LOG(ERROR) << "WriteTask::operator() => Write Error";
        std::abort();
      }
    }
  }
}

SendTask::SendTask(int outfd, std::string pathname, off64_t completed, size_t total)
    : Task(outfd, completed, total)
{
  int fd = ::open(pathname.c_str(), O_RDONLY, 0644) < 0;
  if (fd) {
    LOG(ERROR) << "SendTask::SendTask() => Open File Error";
    std::abort();
  }

  infd_ = fd;
}

SendTask::~SendTask()
{
  if (::close(infd_)) {
    LOG(ERROR) << "SendTask::~SendTask() => Close File Error";
    std::abort();
  }
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
        LOG(ERROR) << "SendTask::operator() => SendFile Error";
        std::abort();
      }
    }
  }
}