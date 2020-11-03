#include "../include/func.h"
#include "../include/threadpool.h"

int epollAddFd(int epfd, int fd){
    struct epoll_event event;
    memset(&event, 0, sizeof(event));

    event.data.fd = fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}

int epollAddTimeFd(int epfd, int fd){
    struct epoll_event event;
    memset(&event, 0, sizeof(event));

    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}
