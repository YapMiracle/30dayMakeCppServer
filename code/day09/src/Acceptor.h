//
// Created by mirac on 2022/10/20.
//

#ifndef UNTITLED_ACCEPTOR_H
#define UNTITLED_ACCEPTOR_H

#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor {
private:
    EventLoop* loop;
    Socket* sock;
    InetAddress* addr;
    Channel* acceptChannel;
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};


#endif //UNTITLED_ACCEPTOR_H
