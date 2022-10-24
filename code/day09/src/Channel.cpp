//
// Created by mirac on 2022/10/11.
//

#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* _loop, int _fd): loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false) {

}

Channel::~Channel() {

}

// 回调函数
void Channel::handleEvent() {
    callback();
}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd() const {
    return fd;
}

uint32_t Channel::getEvents() const {
    return events;
}

//void Channel::setEvents(uint32_t events) {
//    Channel::events = events;
//}

uint32_t Channel::getRevents() const {
    return revents;
}

void Channel::setRevents(uint32_t revents) {
    Channel::revents = revents;
}

bool Channel::isInEpoll() const {
    return inEpoll;
}

void Channel::setInEpoll() {
    inEpoll = true;
}

/**
 * 设置回调函数
 * @param _cb
 */
void Channel::setCallback(std::function<void()> _cb){
    callback = _cb;
}
