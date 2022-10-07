//
// Created by mirac on 2022/10/4.
//

#ifndef UNTITLED_SOCKET_H
#define UNTITLED_SOCKET_H

class InetAddress;
class Socket{
private:
    int listen_fd;
    int client_fd;

public:
    Socket();
    ~Socket();
    void bind(InetAddress*);
    void setnonblocking();

    int accept(InetAddress*);
    void listen();
    int getListen_fd();
    int getClient_fd();
};
#endif //UNTITLED_SOCKET_H
