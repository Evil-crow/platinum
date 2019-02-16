/**
 * Created by Crow on 2/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "core/handler/static_handler.h"

#include <ctime>
#include "net/connection.h"
#include "protocol/http/request.h"
#include "config/config.h"
#include "utility/dir.h"

using namespace platinum;

StaticHandler::StaticHandler(Connection *connection,
                             const http::Request &request,
                             const std::unordered_map<std::string, std::string> &parameters,
                             const std::string &file,
                             const std::string &path) noexcept
    : Handler(connection, request, parameters, file, path)
{
  ;
}

void StaticHandler::Serve()
{
  // 1.Set file from the URL,
  SetResourceFile();

}

void StaticHandler::SetResourceFile()
{
  if (IsValid()) {
    if (IsExist()) {
      if (NotForbidden()) {
        builder_->SetStatusCode(200);
        SetNewFile(file_);                    // Unnecessary steps, but for the integrity of process flow
      } else {
        builder_->SetStatusCode(403);
        SetNewFile("403.hmtl");
      }
    } else {
      builder_->SetStatusCode(404);
      SetNewFile("404.html");
    }
  } else {
    builder_->SetStatusCode(501);
    SetNewFile("501.html");
  }
}

bool StaticHandler::IsValid()
{
  const auto &config = Config::GetInstance();
  const auto &list = config.method_list();
  auto method = request_.method();

  return static_cast<bool>(list.count(method));
}

bool StaticHandler::IsExist()
{
  const auto &config = Config::GetInstance();
  auto www_root = config.www_root();
  auto path = www_root + path_;

  Dir dir(path);
  return dir.IsExist(file_);
}

bool StaticHandler::NotForbidden()
{
  const auto &config = Config::GetInstance();
  const auto &list = config.forbidden_resource();
  auto www_root = config.www_root();
  auto path = www_root + path_;
  for (const auto &var : list) {
    if (path.find(var) != std::string::npos)
      return true;
  }

  return false;
}

void StaticHandler::SetBasicHeaders()
{
  // Set these headers: Date, Server, Connection, Content-Type
  // Header: Date
  char time[30];                      // 30 Bytes is already enough !
  auto t = std::time(nullptr);
  std::strftime(time, sizeof(time), "%a, %d %b %Y %H:%M:%S GMT",std::localtime(&t));

  // Header: Conenction
  auto connection_str = request_.header("Connection");

  // Header: Content-Type
  auto type =

  builder_->SetHeader("Server", "platinum/2.0");
  builder_->SetHeader("Date", time);
  builder_->SetHeader("Connection", connection_str);
  builder_->SetHeader("Content-Type", "");
}
