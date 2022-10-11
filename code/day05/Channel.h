//
// Created by mirac on 2022/10/11.
//

#ifndef UNTITLED_CHANNEL_H
#define UNTITLED_CHANNEL_H

#include "Epoll.h"

class Epoll;
class Channel {
private:
    Epoll* ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
public:
    Channel(Epoll* _ep, int _fd);
    ~Channel();

    void enableReading();

    int getFd() const;

    uint32_t getEvents() const;

    //void setEvents(uint32_t events);

    uint32_t getRevents() const;

    void setRevents(uint32_t revents);

    bool isInEpoll() const;

    void setInEpoll();
};


#endif //UNTITLED_CHANNEL_H
