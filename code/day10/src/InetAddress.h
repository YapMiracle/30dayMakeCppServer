//
// Created by mirac on 2022/10/4.
//

#ifndef UNTITLED_INETADDRESS_H
#define UNTITLED_INETADDRESS_H

#include <arpa/inet.h>
class InetAddress{
private:
    struct sockaddr_in addr;
    socklen_t addr_len;
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
    void setInetAddr(sockaddr_in _addr, socklen_t _addr_len);
    sockaddr_in getAddr();
    socklen_t getAddr_len();
};
#endif //UNTITLED_INETADDRESS_H
