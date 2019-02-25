/*
 * Created by Crow on 8/24/18.
 * Description: Function declaration for server;
 * Function:
 *          void epoll_server(in_port_t port);                      // from the main() to server process
 *          void epoll_deal(PlatinumServer::epoll &epollobj);      // deal with the I/O event
 */

#ifndef PLATINUMSERVER_SERVER_H
#define PLATINUMSERVER_SERVER_H

#include <arpa/inet.h>
#include "epoll.h"
#include "socket.h"

void epoll_server(in_port_t port);

void epoll_deal(PlatinumServer::socket &listen_socket, PlatinumServer::epoll &epollobj, struct epoll_event *evlist);

#endif //PLATINUMSERVER_SERVER_H
