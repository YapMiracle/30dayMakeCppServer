//
// Created by mirac on 2022/10/4.
//

#ifndef UNTITLED_SOCKET_H
#define UNTITLED_SOCKET_H

class InetAddress;
class Socket{
private:
    int fd;
public:
    Socket();
    Socket(int fd);
    ~Socket();
    void bind(InetAddress*);
    void setnonblocking();

    int accept(InetAddress*);
    void listen();
    int getFd();
    void connect(InetAddress* _addr);
};
#endif //UNTITLED_SOCKET_H
