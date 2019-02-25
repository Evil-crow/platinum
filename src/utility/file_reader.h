/**
 * Created by Crow on 2/17/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  FileReader is used to read file for static resource
 *          FileReader() -> while (!Complete()) -> Read() -> END
 */

#ifndef PLATINUM_UTILITY_FILE_READER_H
#define PLATINUM_UTILITY_FILE_READER_H

#include <string>

namespace platinum {

class FileReader {
 public:
  explicit FileReader(const char *filename) noexcept ;
  explicit FileReader(const std::string &filename) noexcept ;
  ~FileReader();
  ssize_t Read(unsigned char *buffer, size_t count);
  ssize_t Size();
  bool Complete();

 private:
  bool Open();

  int fd_;
  std::string filename_;
  ssize_t pos_;
  ssize_t size_;
};

}

#endif //PLATINUM_UTILITY_FILE_READER_H
