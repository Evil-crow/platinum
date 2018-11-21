//
// Created by Crow on 11/21/18.
//

#ifndef PLATINUM_SOCKETOPS_H
#define PLATINUM_SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>

namespace platinum {

namespace socket {

enum class SocketDomain {
  INET = AF_INET,
  INET6 = AF_INET6,
};

enum class SocketType {
  STREAM = SOCK_STREAM,
  DGRAM = SOCK_DGRAM,
};

int socket_(SocketDomain domain, SocketType type);

int bind_(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int listen_(int sockfd, int backlog);

int accept_(int sockfd, sockaddr *addr, socklen_t *addrlen);

int getsockname_(int sockfd, sockaddr *addr, socklen_t *addrlen);

int getpeername_(int sockfd, sockaddr *addr, socklen_t *addrlen);

int getsocketopt_(int sockfd, int level, int optname, void *optval, socklen_t *optlen);

int setsockopt_(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

}

}


#endif //PLATINUM_SOCKETOPS_H
