/*
 * @filename:    client.cc
 * @author:      Crow
 * @date:        12/04/2018 21:07:44
 * @description:
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        std::abort();

    sockaddr_in socketaddr{};
    socketaddr.sin_family = AF_INET;
    socketaddr.sin_port = htons(9877);
    socketaddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    int ret = ::connect(fd, reinterpret_cast<struct sockaddr *>(&socketaddr), sizeof(struct sockaddr));

    if (ret != 0)
        std::abort();
    while (1) {
        usleep(1000000);
        std::cout << "haha" << std::endl;
    }
    std::cout << "connfd: " << ret << std::endl;

    return 0;
}
