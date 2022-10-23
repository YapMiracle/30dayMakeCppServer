//
// Created by mirac on 2022/10/4.
//

#ifndef UNTITLED_EPOLL_H
#define UNTITLED_EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include "Channel.h"

class Channel;
class Epoll {
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    void updateChannel(Channel *);
    // std::vector<epoll_event> poll(int timeout=-1);
    std::vector<Channel*> poll(int timeout=-1);

};
#endif //UNTITLED_EPOLL_H
