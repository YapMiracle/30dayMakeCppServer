//
// Created by mirac on 2022/10/15.
//

#ifndef UNTITLED_EVENTLOOP_H
#define UNTITLED_EVENTLOOP_H

class Epoll;
class Channel;
class EventLoop {
private:
    Epoll *ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);
};


#endif //UNTITLED_EVENTLOOP_H
