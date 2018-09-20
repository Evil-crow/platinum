/*
 * Created by Crow on 8/24/18.
 * Description: This file is so important in PlatinumServer, It's used to deal with the HTTP affair
 *          It bases on <httpxx/Request.hpp>, <httpxx/ResponseBuilder.hpp> and "http_response.h"
 *          to deal with the HTTP-affair, by parse the recv-data and build-response
 */

#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <cstddef>
#include <sys/socket.h>
#include <httpxx/Request.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "http_affair.h"
#include "http_response.h"


bool is_static(http::Request &request)
{
    const std::string &url(request.url());

    if (url.find('?') != std::string::npos)
        return false;
    return true;
}

const std::string get_dir(std::string &url)
{
    if (!url.empty()) {
        auto pos = url.find_last_of('/');
        return std::string(url, 0, pos);
    }

    return std::string();
}

const std::string get_filename(std::string &url)
{
    if (!url.empty()) {
        auto pos = url.find_last_of('/');
        return std::string(url, pos + 1);
    }

    return std::string();
}

bool get_request(http::Request &request, PlatinumServer::socket &conn_sock)
{
    char buffer[100];

    while (!request.complete()) {
        const ssize_t transferred = ::recv(conn_sock.get_fd(), buffer, 100, 0);
        if (transferred == 0)
            break;
        request.feed(buffer, (size_t)transferred);
        std::memset(buffer, 0, 100);
    }

    if (!request.complete())
        return false;

    return true;
}

long int find_file(const std::string &request_url)
{
    DIR *dp = nullptr;
    struct dirent *p = nullptr;
    bool flag = false;

    std::string url(".");
    url += request_url;

    const std::string filename = get_filename(url);

    try {
        dp = ::opendir(get_dir(url).c_str());

        std::cout << get_dir(url) << std::endl;
        if (dp == nullptr)
            throw std::runtime_error("opendir() error");
    } catch (std::runtime_error &re) {
        std::cout << "Runtime Error: " << re.what() << std::endl;
        std::abort();
    }

    while ((p = readdir(dp)) != nullptr) {
        std::cout << filename << " -> " << p->d_name << std::endl;
        if (std::strcmp(p->d_name, filename.c_str()) == 0) {
            flag = true;
            std::cout << "find it!" << std::endl;
            break;
        }
    }
    ::closedir(dp);
    if (!flag)
        return -1;

    struct stat stat_buf{};
    long int file_size;
    try {
        std::cout << url.c_str() << std::endl;
        int ret = ::lstat(url.c_str(), &stat_buf);
        if (ret == -1) {
            std::cout << "error code: " << errno << std::endl;
            throw std::runtime_error("lstat error");
        }
        file_size = stat_buf.st_size;
    } catch(std::runtime_error &re) {
        std::cout << "Runtime Error: " << re.what() << std::endl;
        std::abort();
    }

    return file_size;
}

bool is_alive(http::Request &request)
{
    std::string str = request.header("Connection");
    if (str == "Keep-Alive")
        return true;
    return false;
}

bool build_response(PlatinumServer::http_response &response, http::Request &request)
{
    std::string url(".");
    url += request.url();

    response.set_major_version(1);
    response.set_minor_version(1);
    response.set_header("Server", "PlatinumServer/1.0");

    long int file_size = 0;
    if ((file_size = find_file(request.url())) == -1) {
        response.set_status(404);
        url = "./404.html";
        file_size = find_file("/404.html");
    }
    std::cout << "file_size: " << file_size << std::endl;
    std::cout << "Url: " << url << std::endl;
    response.set_status(200);
    response.set_header("Content-Length", std::to_string(file_size));

    std::string response_body;
    long int sum = 0;
    char buffer[251];
    int fd = ::open(url.c_str(), O_RDWR, 0777);
    while (sum != file_size) {
        ::memset(buffer, 0, sizeof(buffer));
        std::cout << sum << " -> " << file_size << std::endl;
        try {
            auto temp = ::read(fd, buffer, 250);
            std::cout << "buffer_size: " << temp << std::endl << buffer << std::endl;
            if (temp == -1) {
                throw std::runtime_error("READ ERROR");
            }
            sum += temp;
            response_body += buffer;
        } catch (std::runtime_error &re) {
            std::cout << "Runtime Error: " << re.what() << std::endl;
            std::abort();
        }
    }
    response.set_body(std::move(response_body));

    if (!is_alive(request))
        response.set_header("Connection", "close");
    response.set_header("Connection", "Keep-Alive");

    return true;
}

void deal_http_affair(PlatinumServer::socket &conn_socket, PlatinumServer::epoll &epollobj)
{
    http::Request request;
    PlatinumServer::http_response response;

    // 1. get the HTTP Request
    ::get_request(request, conn_socket);

    // Display the Request
    std::cout << "xxxxxxxxx Completely Start xxxxxxxxxxxx" << std::endl
              << "method: " << request.method().to_string() << std::endl
              << "Url: " << request.url() << std::endl
              << "Headers: " << std::endl
              << "Connection: " << request.header("Connection") << std::endl
              << "Host: " << request.header("Host") << std::endl
              << "Body: " << std::endl
              << "xxxxxxxxx Completely end xxxxxxxxxxxxxx" << std::endl;

    // 2. judge the static/dynamic request
    if (!is_static(request)) {
        std::cout << "Now, We can't support the dynamic request" << std::endl;
        return ;
    }

    // 3. find the file for request (in ::build_response())
    // 4. build the response
    ::build_response(response, request);
    response.build();

    std::string _res = response.get_response();
    std::cout << _res << std::endl;
    // std::abort();

    // 5. send the response
    std::string res = response.get_response();
    ::send(conn_socket.get_fd(), res.c_str(), res.size(), 0);
    std::cout << "get out!" << std::endl;

    if (request.header("Connection") == "close") {
        epollobj.del(conn_socket.get_fd());
        ::close(conn_socket.get_fd());
    }

    // Register the EPOLL event
    epollobj.modify(conn_socket.get_fd(), { EPOLLIN, EPOLLONESHOT });
}