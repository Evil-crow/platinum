#include <iostream>
#include "server.h"
#include "thread_pool.h"

int main(int argc, char *argv[])
{
    epoll_server(8090);
    return 0;
}