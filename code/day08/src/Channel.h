//
// Created by mirac on 2022/10/11.
//

#ifndef UNTITLED_CHANNEL_H
#define UNTITLED_CHANNEL_H

#include "Epoll.h"
#include "EventLoop.h"
#include <functional>

class Epoll;
class Channel {
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;
public:
    Channel(EventLoop* _loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd() const;
    uint32_t getEvents() const;
    //void setEvents(uint32_t events);
    uint32_t getRevents() const;
    void setRevents(uint32_t revents);
    bool isInEpoll() const;
    void setInEpoll();
    void setCallback(std::function<void()>);
};


#endif //UNTITLED_CHANNEL_H
