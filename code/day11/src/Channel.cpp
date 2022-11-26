//
// Created by mirac on 2022/10/11.
//

#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* _loop, int _fd)
        : loop(_loop), fd(_fd), events(0), ready(0),
        inEpoll(false),useThreadPool(false) {

}

Channel::~Channel() {
    if(fd!=-1){
        close(fd);
        fd=-1;
    }
}

// 回调函数
void Channel::handleEvent() {
    if(ready & (EPOLLIN | EPOLLPRI)){
        if(useThreadPool){
            loop->addThread(readCallback);
        } else{
            readCallback();
        }
    }
    if(ready & (EPOLLOUT)){
        if(useThreadPool){
            loop->addThread(writeCallback);
        } else {
            writeCallback();
        }
    }
}

void Channel::enableRead() {
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

void Channel::useET() {
    events |=EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() const {
    return fd;
}

uint32_t Channel::getEvents() const {
    return events;
}

uint32_t Channel::getReady() const {
    return ready;
}

void Channel::setReady(uint32_t ev) {
    ready = ev;
}
bool Channel::isInEpoll() const {
    return inEpoll;
}

void Channel::setInEpoll(bool _in) {
    inEpoll = _in;
}

/**
 * 设置回调函数
 * @param _cb
 */
void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}
void Channel::setUseThreadPool(bool use){
    useThreadPool = use;
}
