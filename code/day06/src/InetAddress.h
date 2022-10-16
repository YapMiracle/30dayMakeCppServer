//
// Created by mirac on 2022/10/4.
//

#ifndef UNTITLED_INETADDRESS_H
#define UNTITLED_INETADDRESS_H

#include <arpa/inet.h>
class InetAddress{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};
#endif //UNTITLED_INETADDRESS_H
