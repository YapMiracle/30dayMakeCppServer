//
// Created by mirac on 2022/10/15.
//

#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"

/**
 * 管理一个Epoll
 */
EventLoop::EventLoop():ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

/**
 * 处理事件循环
 */
void EventLoop::loop() {
    while(!quit){
        std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it!=chs.end(); it++) {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch){
    ep->updateChannel(ch);
}