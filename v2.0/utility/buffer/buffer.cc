//
// Created by Crow on 12/17/18.
//

#include "buffer.h"

#include <unistd.h>
#include <cstring>
#include <algorithm>

#include "utility/logger.h"

using namespace platinum;

Buffer::Buffer()
    : read_index_(0),
      write_index_(0)
{
  buffer_.reserve(1024);
}

size_t Buffer::ReadFd(int fd)
{
  ssize_t val(0);
  char data[65535]{};

  while (true) {
    val = ::read(fd, data, 65535);
    if (val == 0) {
      break;
    } else if (val < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        LOG(INFO) << "Buffer::ReadFd OK";
        break;
      } else {
        LOG(ERROR) << "Buffer::ReadFd ERROR!";
        std::abort();
      }
    } else {
      FillData(reinterpret_cast<void *>(&data), val);
    }
  }

  return val == 0 ? val : Readable();
}

void Buffer::FillData(void *data, size_t length)
{
  if (Writeable() + FrontSpace() < length) {                      // 1. Space < data length, need to resize()
    buffer_.resize(FrontSpace() + Readable() + length);           // Extended space
  } else if (Writeable() < length
      && Writeable() + FrontSpace() >= length) {                  // 2. Total space > data length, need to move data()
    MoveData();
  }

  // After making the space fit, Fill the data into buffer.
  ::memcpy(buffer_.data() + write_index_, data, length);
//  std::copy(data, data + length, buffer_.data() + write_index_);
  write_index_ += length;
}

void Buffer::MoveData()
{
  auto iter(buffer_.begin());
  auto length(write_index_ - read_index_);               // data length

  // use std::copy to move data to the front-end
  std::copy(iter + read_index_, iter + write_index_ + 1, iter);
  read_index_ = 0;
  write_index_ = read_index_ + length;
}

Buffer::const_iter Buffer::BufferBegin() const
{
  return buffer_.cbegin() + read_index_;
}

Buffer::const_iter Buffer::BufferEnd() const
{
  return buffer_.cbegin() + write_index_;
}

void Buffer::Reprepare()
{
  read_index_ = 0;
  write_index_ = 0;
}