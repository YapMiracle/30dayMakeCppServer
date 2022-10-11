//
// Created by mirac on 2022/10/11.
//

#include "Channel.h"

Channel::Channel(Epoll* _ep, int _fd): ep(_ep), fd(_fd), events(0), revents(0), inEpoll(false) {

}

Channel::~Channel() {

}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
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
