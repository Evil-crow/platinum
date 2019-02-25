/**
 * Created by Crow on 1/5/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  Test HTTP Request Parser, use Class RequestParser
 */

#include <iostream>
#include "protocol/http/request.h"
#include "../protocol/http/request_parser.h"

int main()
{
  std::string s1("POST ");
  std::string s2("/ HT");
  std::string s3("TP/1");
  std::string s4(".1\r\n");
  std::string s5("Host: C");
  std::string s6("row\r\nContent-Length: 5\r\n");
  std::string s7("\r\na&b=c");

  std::vector<unsigned char> request1(s1.cbegin(), s1.cend());
  std::vector<unsigned char> request2(s2.cbegin(), s2.cend());
  std::vector<unsigned char> request3(s3.cbegin(), s3.cend());
  std::vector<unsigned char> request4(s4.cbegin(), s4.cend());
  std::vector<unsigned char> request5(s5.cbegin(), s5.cend());
  std::vector<unsigned char> request6(s6.cbegin(), s6.cend());
  std::vector<unsigned char> request7(s7.cbegin(), s7.cend());

  platinum::http::RequestParser parser;

  parser.feed(request1.cbegin(), request1.size());
  if (parser.Complete())
    std::cout << "1 OK\n";
  parser.feed(request2.cbegin(), request2.size());
  if (parser.Complete())
    std::cout << "2 OK\n";
  parser.feed(request3.cbegin(), request3.size());
  if (parser.Complete())
    std::cout << "3 OK\n";
  parser.feed(request4.cbegin(), request4.size());
  if (parser.Complete())
    std::cout << "4 OK\n";
  parser.feed(request5.cbegin(), request5.size());
  if (parser.Complete())
    std::cout << "5 OK\n";
  parser.feed(request6.cbegin(), request6.size());
  if (parser.Complete())
    std::cout << "6 OK\n";
  parser.feed(request7.cbegin(), request7.size());
  if (parser.Complete())
    std::cout << "7 OK\n";

  auto request = parser.GetRequest();
  std::cout << request.method() << std::endl;
  std::cout << request.url() << std::endl;
  std::cout << request.major_version() << "." << request.minor_version() << std::endl;
  auto headers = request.headers();
  for (const auto &var : headers)
    std::cout << var.first << ": " << var.second << std::endl;
  auto body = request.body();
  for (const auto &var : body)
    std::cout << var;
  std::cout << '\n';
}