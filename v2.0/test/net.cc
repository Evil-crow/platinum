//
// Created by Crow on 12/5/18.
//

#include <unistd.h>
#include <iostream>
#include "net/ip_address.h"
#include "net/socket.h"

int main(int argc, char *argv[])
{
  platinum::IPAddress addr(9877);
  platinum::Socket acceptor;
  acceptor.SetReusePort(true);
  acceptor.SetTcpNoDelay(true);
  acceptor.Bind(addr);
  acceptor.Listen();


  while (true) {
    int fd = acceptor.Accept();
    std::cout << fd << std::endl;
    if (fd > 0)
      break;
  }

  return 0;
}