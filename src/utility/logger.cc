/**
 * Created by Crow on 11/19/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to immplmente Logger,
 *         use static to immplmente Singleton (C++11)
 *         do not use double-lock
 */

#include "logger.h"

#include <ctime>
#include <fstream>
#include <mutex>

#include "config/config.h"

using namespace platinum;

inline Logger &LoggerOn::GetInstance()
{
  static LoggerOn instance;
  return instance;
}

void LoggerOn::operator<<(const std::string &&str)
{
  std::lock_guard<std::mutex> lk(mtx);
  const auto &log_priority = this->priority_;

  char time[100];
  std::time_t t_ = std::time(nullptr);
  std::strftime(time, sizeof(time), "%Y/%m/%d %H:%M:%S ", std::localtime(&t_));

  std::fstream of("../platinum.log", std::ios::out | std::ios::app);
  of << time << "[" << log_priority << "]: ";
  of << str << std::endl;

  of.close();
}

inline void LoggerOn::SetLevel(const std::string &&priority) {
  this->priority_ = priority;
}

inline Logger &LoggerOff::GetInstance()
{
  static LoggerOff instance;
  return instance;
}

Logger &Logger::GetLog() {
  auto &config = Config::GetInstance();

  if (config.IsLogOn()) {
    return LoggerOn::GetInstance();
  } else {
    return LoggerOff::GetInstance();
  }
}
