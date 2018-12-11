/*
 * Created by Crow on 8/23/18.
 * Description: Any high performance can't leave LInux::epoll() at now
 *             this is an encapsulation for epoll() on Linux
 * API:
 *          (non)  epoll();                           // Constructor ( epoll_create() )
 *          bool   add(int fd, ...);                  // EPOLL_CTL_ADD
 *          bool   modify(int fd, ...);               // EPOLL_CTL_MOD
 *          bool   modify(int fd);                    // EPOLL_CTL_DEL
 *          int    wait(struct epoll_event *, int)    // epoll_wait();
 */


#ifndef PLATINUMSERVER_EPOLL_H
#define PLATINUMSERVER_EPOLL_H

#include <sys/epoll.h>
#include <initializer_list>

namespace PlatinumServer {
class epoll {
public:
    epoll() noexcept;
    epoll(const epoll &) = default;
    epoll &operator=(const epoll &) = delete;
    int get_io_count();
    bool add(int fd, std::initializer_list<uint32_t> events);
    bool modify(int fd, std::initializer_list<uint32_t> events);
    bool del(int fd);
    bool wait(struct epoll_event *events, int max_events, int timeout);
private:
    int epoll_fd;
    int io_count;

    void set(struct epoll_event &, const std::initializer_list<uint32_t> &);
};
}

#endif //PLATINUMSERVER_EPOLL_H
