/**
 * Created by Crow on 2/13/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "utility/dir.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>

#include "utility/logger.h"

using namespace platinum;

Dir::Dir(const char *pathname) noexcept
  : dp_(nullptr)
{
  auto dp = ::opendir(pathname);
  if (dp == nullptr) {
    LOG(ERROR) << "Dir::Dir() => Open Dir Error";
  } else {
    dp_ = dp;
  }
}

Dir::Dir(const std::string &pathname) : Dir(pathname.c_str()) { ; }

Dir::~Dir()
{
  try {
    if (dp_) {
      auto ret = ::closedir(dp_);
      if (ret < 0)
        throw std::runtime_error(strerror(errno));
    } else {
      return ;
    }
  } catch (const std::exception &e) {
    LOG(ERROR) << e.what();
  }
}

bool Dir::IsExist(const char *filename)
{
  if (dp_) {
    struct dirent *dirp{nullptr};
    while ((dirp = ::readdir(dp_)) != nullptr) {
      if (std::strcmp(dirp->d_name, filename) == 0) {
        return true;
      } else {
        continue;
      }
    }
    return false;
  } else {
    return false;
  }
}

bool Dir::IsExist(const std::string &filename)
{
  return IsExist(filename.c_str());
}