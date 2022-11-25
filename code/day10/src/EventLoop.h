//
// Created by mirac on 2022/10/15.
//

#ifndef UNTITLED_EVENTLOOP_H
#define UNTITLED_EVENTLOOP_H

#include <functional>
class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel*);
    void addThread(std::function<void()>);
};


#endif //UNTITLED_EVENTLOOP_H
