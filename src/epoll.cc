/*
 * Created by Crow on 8/23/18.
 * Description: the implement of epoll class
 */

#include <stdexcept>
#include "epoll.h"

#define ERR_HANDLE(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

PlatinumServer::epoll::epoll() noexcept
{
    io_count = 0;
    epoll_fd = ::epoll_create(10);
    if (epoll_fd == -1)
        ERR_HANDLE("epoll_create");
}

void PlatinumServer::epoll::set(struct epoll_event &event, std::initializer_list<unsigned int> &ev)
{
    for (unsigned int iter : ev)
        event.events |= iter;
}

bool PlatinumServer::epoll::add(int fd, std::initializer_list<unsigned int> events)
{
    struct epoll_event event{ };

    set(event, events);
    event.data.fd = fd;
    if (::epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
        ERR_HANDLE("epoll_ctl");

    return true;
}

bool PlatinumServer::epoll::modify(int fd, std::initializer_list<unsigned int> events)
{
    struct epoll_event event{ };

    set(event, events);
    event.data.fd = fd;
    if (::epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
        ERR_HANDLE("epoll_ctl");

    return true;
}

bool PlatinumServer::epoll::del(int fd)
{
    if (::epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1)
        ERR_HANDLE("epoll_ctl");

    return true;
}

bool PlatinumServer::epoll::wait(struct epoll_event *events, int max_events, int timeout)
{
    int ret_val;

    if ((ret_val = epoll_wait(this->epoll_fd, events, max_events, timeout)) <= 0) {
        switch (ret_val) {
            case 0: ERR_HANDLE("epoll_wait");
                break;
            case -1: ERR_HANDLE("epoll_wait");
                break;
            default:break;
        }
    }

    this->io_count = ret_val;            // get the io_cout;
    return true;
}


