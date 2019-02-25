/**
 * Created by Crow on 2/12/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "protocol/http/response_builder.h"
#include "protocol/http/response.h"

#include <iostream>

int main()
{
  platinum::http::ResponseBuilder builder;

  builder.SetVersionMajor(1);
  builder.SetVersionMinor(0);
  builder.SetStatusCode(200);
  builder.SetHeader("Content-Type", "/text/plain");
  builder.SetHeader("Server", "Platinum");

  auto response = builder.GetResponse();

  std::cout << response.StatusLine() << std::endl;
  auto data = response.Build();
}