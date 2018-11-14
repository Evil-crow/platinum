//
// Created by Crow on 11/14/18.
//

#include <ctime>
#include <iosfwd>
#include <string>
#include <tuple>
#include <string>
#include <fstream>

namespace Platinum {

#ifndef LOGON
#define LOG(priority, journal) (0 && logger << std::make_tuple(priority, journal))
#else
#define LOG(priority, journal) (logger << std::make_tuple(priority, journal))
#endif

class Logger {
 public:
  bool operator<<(const std::tuple<std::string, std::string> &t);
}logger;


bool Logger::operator<<(const std::tuple<std::string, std::string> &t) {
  const auto &log_priority = std::get<0>(t);

  char time[100];
  std::time_t t_ = std::time(nullptr);
  std::strftime(time, sizeof(time), "%Y/%m/%d %H:%M:%S ", std::localtime(&t_));

  std::fstream of("../Platinum.log", std::ios::out | std::ios::app);
  of << time << "[" << log_priority << "]: ";
  of << std::get<1>(t) << std::endl;

  of.close();
  return true;
}
}

