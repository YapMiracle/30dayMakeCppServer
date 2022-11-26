//
// Created by mirac on 2022/10/15.
//

#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"

/**
 * 管理一个Epoll，
 * 和一个事件处理线程池
 */
EventLoop::EventLoop():ep(nullptr), threadPool(nullptr), quit(false) {
    ep = new Epoll();
    threadPool = new ThreadPool();// default size =10.
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

void EventLoop::addThread(std::function<void()> func){
    threadPool->add(func);
}