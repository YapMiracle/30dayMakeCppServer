//
// Created by mirac on 2022/10/15.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);
};


#endif //UNTITLED_SERVER_H

