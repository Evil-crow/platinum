/**
 * Created by Crow on 2/17/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This file is for OS file operation
 */

#include "utility/file_reader.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

#include "utility/logger.h"
using namespace platinum;

FileReader::FileReader(const char *filename) noexcept
    : filename_(filename),
      pos_(0),
      size_(0)
{
  auto fd = ::open(filename, O_RDONLY, 0644);
  if (fd < 0) {
    LOG(ERROR) << "FileReader::FileReader() => Open File Error";
  }
  fd_ = fd;

  struct stat buf{};
  int ret = ::lstat(filename, &buf);
  if (ret == 0) {
      size_ = buf.st_size;
  } else {
    LOG(WARN) << "FileReader::FileReader() => Lstat File Error";
  }
}

FileReader::FileReader(const std::string &filename) noexcept
    : FileReader(filename.c_str())
{ ; }

FileReader::~FileReader()
{
  if (Open()) {
    int ret = ::close(fd_);
    if (ret < 0) {
      LOG(ERROR) << "FileReader::~FileReader() => Close File Error";
    }
  }
}


ssize_t FileReader::Read(unsigned char *buffer, size_t count)
{
  ssize_t read_num;
  if (Open()) {
    read_num = ::read(fd_, buffer, count);
    if (read_num > 0)
      pos_ += read_num;
  } else {
    read_num = -1;
  }

  return read_num;
}

ssize_t FileReader::Size()
{
  return size_;
}

bool FileReader::Complete()
{
  return pos_ == size_;
}

bool FileReader::Open()
{
  return fd_ > 0;
}