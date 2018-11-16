//
// Created by Crow on 11/15/18.
//

#include "log.h"

#include <ctime>
#include <fstream>
#include <tuple>

namespace Platinum {

Logger &LoggerOn::GetInstance()
{
  static LoggerOn instance;
  return instance;
}

void LoggerOn::operator<<(const std::string &&vec)
{
  const auto &log_priority = this->priority_;

  char time[100];
  std::time_t t_ = std::time(nullptr);
  std::strftime(time, sizeof(time), "%Y/%m/%d %H:%M:%S ", std::localtime(&t_));

  std::fstream of("../Platinum.log", std::ios::out | std::ios::app);
  of << time << "[" << log_priority << "]: ";
  of << vec << std::endl;

  of.close();
}

void LoggerOn::SetLevel(const std::string &&priority) {
  this->priority_ = priority;
}

Logger &LoggerOff::GetInstance()
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

}