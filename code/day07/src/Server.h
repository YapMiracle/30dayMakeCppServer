//
// Created by mirac on 2022/10/15.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H


class EventLoop;
class Socket;
class Acceptor;
class Server
{
private:
    EventLoop *loop;
    Acceptor* acceptor;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};


#endif //UNTITLED_SERVER_H
