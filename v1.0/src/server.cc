/*
 * Created by Crow on 8/24/18.
 * Description: This file's purpose is to achieve the high performance requirements
 *          The server used thread pool, socket class and epoll-API to improve the
 *          performance, It works under the ET mode of epoll()
 */

#include <arpa/inet.h>
#include "thread_pool.h"
#include "http_affair.h"
#include "server.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
inline void err_handle(const std::string &msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

const int MAX_EPOLL_LIST = 1000;
PlatinumServer::thread_pool pool(8);

void epoll_server(in_port_t port)
{
    struct epoll_event evlist[MAX_EPOLL_LIST];
    PlatinumServer::epoll epoll;
    PlatinumServer::socket listen_socket(port);// create socket object
    listen_socket.connect();                                                                    // finish the bind() and listen()

    epoll.add(listen_socket.get_fd(), { EPOLLIN });                                           // add listen fd to epoll event table
    while (true) {
        epoll.wait(evlist, MAX_EPOLL_LIST, -1);
        switch (epoll.get_io_count()) {
            case -1:                                                                          // epoll_wait() error
                err_handle("epoll_wait");
                break;
            default:                                                                          // IO event
                epoll_deal(listen_socket, epoll, evlist);                                     // deal with the epoll I/O event
                break;
        }
    }
}

void epoll_deal(PlatinumServer::socket &listen_socket, PlatinumServer::epoll &epollobj, struct epoll_event *evlist)
{
    for (int i = 0; i < epollobj.get_io_count(); ++i) {
        if ((evlist[i].data.fd == listen_socket.get_fd()) && (evlist[i].events & EPOLLIN)) {                                         // I/O event happen on listen_fd;
            int conn_fd = listen_socket.accept();
            epollobj.add(conn_fd, { EPOLLIN, EPOLLONESHOT });                           // add event
        } else if ((evlist[i].events & EPOLLIN)) {                                      // general I/O event, the HTTP affair
            PlatinumServer::socket conn_socket(evlist[i].data.fd);
            deal_http_affair(conn_socket, epollobj);
            // auto task = [&](){ return func(conn_socket, epollobj); };
            // pool.submit_task(task);
        }
    }
}
#pragma clang diagnostic pop