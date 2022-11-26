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
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(EventLoop* _loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();
    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getReady() const;
    void setReady(uint32_t);
    void useET();
    bool isInEpoll() const;
    void setInEpoll(bool _in = true);
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};


#endif //UNTITLED_CHANNEL_H
