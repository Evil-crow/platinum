/*
 * Created by Crow on 8/24/18.
 * Description: the headers for HTTP affairs
 * API:
 *      void deal_http_affair(PlatinumServer::socket &conn_fd, PlatinumServer::epoll &epollobj);
 *      bool is_static(http::Request &request);
 */


#ifndef PLATINUMSERVER_HTTP_AFFAIR_H
#define PLATINUMSERVER_HTTP_AFFAIR_H

#include <httpxx/Request.hpp>
#include "socket.h"
#include "epoll.h"
#include "http_response.h"

/*
 * Originally intended to be implemented as a member function,
 * but modifying the library code is unreasonable,
 * and inheritance for a member function is not appropriate,
 * so it is implemented as a global function.
 */

bool is_static(http::Request &request);

const std::string get_dir(std::string &url);

const std::string get_filename(std::string &url);

bool get_request(http::Request &request, PlatinumServer::socket &conn_sock);

long int find_file(const std::string &request_url);

bool is_alive(http::Request &request);

bool build_response(PlatinumServer::http_response &response, http::Request &request);

bool send_response(http::Request &request,
        PlatinumServer::http_response &response,
        PlatinumServer::socket &conn_socket, bool isExist);

void deal_http_affair(PlatinumServer::socket &conn_socket, PlatinumServer::epoll &epollobj);
#endif //PLATINUMSERVER_HTTP_AFFAIR_H
