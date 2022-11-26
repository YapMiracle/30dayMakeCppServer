//
// Created by mirac on 2022/10/4.
//

#include <strings.h>
#include <unistd.h>
#include "Epoll.h"
#include "util.h"
#define MAX_EVENTS 1000

/**
 * epoll_cteate1(0)
 */
Epoll::Epoll() : epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    error_if(epfd == -1, "epoll_create1 failed");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

/**
 * 将fd close（）
 */
Epoll::~Epoll() {
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

/**
 * 添加一个fd，EPOLL_CTL_ADD
 * @param fd
 * @param op
 */
void Epoll::addFd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    error_if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll_ctl add failed");
}

void Epoll::updateChannel(Channel *channel) {
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->isInEpoll()){
        error_if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add failed");
        channel->setInEpoll();
    } else {
        error_if(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) ==-1, "epoll modify failed");
    }
}

/**
 * 设置有事件发生的Channel，检测共用的那个ep的树上有多少有事件发生的fd
 * @param timeout
 * @return
 */
std::vector<Channel *> Epoll::poll(int timeout) {
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    error_if(nfds==-1, "epoll wait error");
    for(int i=0; i<nfds; i++) {
        Channel* ch = (Channel*) events[i].data.ptr;
        ch->setReady(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

/**
 * 返回一个已经就绪的event vector
 * @param timeout
 * @return
 */
//std::vector<epoll_event> Epoll::poll(int timeout) {
//    std::vector<epoll_event> activeEvents;
//    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
//    error_if(nfds == -1, "epoll_wait failed");
//    for(int i=0; i<nfds; ++i) {
//        activeEvents.push_back(events[i]);
//    }
//    return activeEvents;
//}


