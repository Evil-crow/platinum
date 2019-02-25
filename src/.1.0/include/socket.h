/*
 * Created by Crow on 8/22/18.
 * Description: the class of socket APIs
 * API:
 *      (non) socket::socket(domain, type, port);                 // constructor
 *      (non) socket::~socket();                                  // destructor
 *      void  socket::connect();                                  // bind and listen on the sock_fd
 *      int   socket::get_fd();                                   // get the listen_fd to do something (epoll)
 *      int   socket::accept();                                   // get the conn_fd to do something (affair)
 */


#ifndef PLATINUMSERVER_SOCKET_H
#define PLATINUMSERVER_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

namespace PlatinumServer {
class socket {
public:
    socket() = default;
    explicit socket(in_port_t _port);
    explicit socket(int sock_fd);
    socket(const socket &) = default;
    socket &operator=(const socket &) = delete;
    ~socket();
    void close();
    void connect();
    int accept();
    inline int get_fd() const
    {
        return this->sock_fd;
    }
private:
    int sock_fd;
    struct sockaddr_in sock_sockaddr;

    bool bind();
    bool listen();

    constexpr int backlog(){ return 1024; }
};
}

#endif //PLATINUMSERVER_SOCKET_H
