/**
 * Created by Crow on 2/13/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#ifndef PLATINUM_UTILITY_DIR_H
#define PLATINUM_UTILITY_DIR_H

#include <string>
#include <dirent.h>

namespace platinum {

class Dir {
 public:
  explicit Dir(const char *pathname) noexcept ;
  explicit Dir(const std::string &pathname) noexcept ;
  ~Dir();

  bool IsExist(const char *filename);
  bool IsExist(const std::string &filename);

 private:
  DIR *dp_;
};

}

#endif //PLATINUM_UTILITY_DIR_H
