#include <iostream>
#include <csignal>

#include "core/platinum.h"

int main(int argc, char *argv[])
{
  platinum::Platinum server(1);
  server.Start();
  server.exec();

  return 0;
}