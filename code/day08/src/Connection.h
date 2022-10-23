//
// Created by mirac on 2022/10/23.
//

#ifndef UNTITLED_CONNECTION_H
#define UNTITLED_CONNECTION_H

#include <functional>
class EventLoop;
class Socket;
class Channel;
class Connection {
private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connection(EventLoop* _loop, Socket* _sock);
    ~Connection();

    void echo(int);
    void setDeleteConnectionCallback(std::function<void (Socket*)>);
};


#endif //UNTITLED_CONNECTION_H
