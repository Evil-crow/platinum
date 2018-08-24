/*
 * Created by Crow on 8/24/18.
 * Description: This file's purpose is to achieve the high performance requirements
 *          The server used thread pool, socket class and epoll-API to improve the
 *          performance, It works under the ET mode of epoll()
 */

#include <arpa/inet.h>
#include <array>
#include <unistd.h>
#include "thread_pool.h"
#include "http_affair.h"
#include "server.h"

#define ERR_HANDLE(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

const int MAX_EPOLL_LIST = 1000;

void epoll_server(in_port_t port)
{
    PlatinumServer::socket listen_socket(port);       // create socket object
    listen_socket.set_non_blocking();              // set O_NONBLOCK for epoll()[ET],
    listen_socket.connect();                       // finish the bind() and listen()

    struct epoll_event evlist[MAX_EPOLL_LIST];
    PlatinumServer::epoll epoll;
    epoll.add(listen_socket.get_fd(), { EPOLLIN, EPOLLET });                     // add listen fd to epoll event table

    while (true) {
        epoll.wait(evlist, MAX_EPOLL_LIST, -1);
        switch (epoll.get_io_count()) {
            case -1:                                                                          // epoll_wait() error
                ERR_HANDLE("epoll_wait");
                break;
            case 0:                                                                           // no-event
                break;
            default:                                                                          // IO event
                epoll_deal(listen_socket.get_fd(), epoll
                           , evlist);                                    // deal with the epoll I/O event
                break;
        }
    }
}

void epoll_deal(int listen_fd, PlatinumServer::epoll &epollobj, struct epoll_event *evlist)
{
    PlatinumServer::socket listen_socket(listen_fd);

    for (int i = 0; i < epollobj.get_io_count(); ++i) {
        if ((evlist[i].data.fd == listen_socket.get_fd())
                && (evlist[i].events & EPOLLIN)) {             // I/O event happen on listen_fd;
            int conn_fd;

            // while(accept()) for epoll()[ET];
            while ((conn_fd = listen_socket.accept()) > -1) {
                std::cout << "conn_fd: " << conn_fd << "errno: " << errno << std::endl;
                PlatinumServer::socket conn_socket(conn_fd);                // Construct the socket by conn_fd
                conn_socket.set_non_blocking();                             // set NON-BLOCK
                epollobj.add(conn_socket.get_fd(), { EPOLLIN, EPOLLET, EPOLLONESHOT });         // add event
            }
        }
        else if ((evlist[i].events & EPOLLRDHUP)
                || (evlist[i].events & EPOLLERR)) {            // the client has hang up
            std::cout << "client hang out" << std::endl;
            epollobj.del(evlist[i].data.fd);
            ::close(evlist[i].data.fd);
        }
        else {
            if ((evlist[i].events & EPOLLIN)
                    && !(evlist[i].events & EPOLLRDHUP)) {            // general I/O event, the HTTP affair
                std::cout << "got in the pool" << std::endl;
                deal_http_affair(PlatinumServer::socket(evlist[i].data.fd), epollobj);
            }
        }
    }
}