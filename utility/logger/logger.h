//
// Created by Crow on 11/19/18.
//

#ifndef PLATINUM_LOG_H
#define PLATINUM_LOG_H

#include <string>
#include <vector>
#include <mutex>

#include "config.h"

namespace platinum {

#define ERROR "ERROR"
#define WARN "WARN"
#define INFO "INFO"
#define LOG(PRIORITY) (Logger::GetLog()).SetLevel(PRIORITY); Logger::GetLog()

class Logger {
 public:
  Logger(const Logger &) = delete;
  void operator=(const Logger &) = delete;

  static Logger &GetLog();
  virtual void operator<<(const std::string &&vec) = 0;
  virtual void SetLevel(const std::string &&priority) = 0;

 protected:
  Logger() = default;
  virtual ~Logger() = default;

};

class LoggerOn : public Logger {
 public:
  static Logger &GetInstance();
  void operator<<(const std::string &&vec) override;
  void SetLevel(const std::string &&priority) override;

 private:
  LoggerOn() = default;
  ~LoggerOn() override = default;

  std::string priority_;
  std::mutex mtx;
};

class LoggerOff : public Logger {
 public:
  static Logger &GetInstance();
  void operator<<(const std::string &&vec) override {}  // Class LoggerOff is defined to do nothing ! (For Logger is off)
  void SetLevel(const std::string &&priority) override {}

 private:
  LoggerOff() = default;
  ~LoggerOff() override = default;

  // Class LoggerOff needn't to provide the private member data --priority
};

}

#endif //PLATINUM_LOG_H
