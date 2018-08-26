/*
 * Created by Crow on 8/24/18.
 * Description: the headers for HTTP affairs
 * API:
 *      void deal_http_affair(PlatinumServer::socket &conn_fd, PlatinumServer::epoll &epollobj);
 */


#ifndef PLATINUMSERVER_HTTP_AFFAIR_H
#define PLATINUMSERVER_HTTP_AFFAIR_H

#include "socket.h"
#include "epoll.h"

void deal_http_affair(PlatinumServer::socket &conn_socket, PlatinumServer::epoll &epollobj);

#endif //PLATINUMSERVER_HTTP_AFFAIR_H
