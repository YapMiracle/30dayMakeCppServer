//
// Created by mirac on 2022/10/4.
//

#include <cstring>
#include "InetAddress.h"

/**
 * 
 */
InetAddress::InetAddress() : addr_len(sizeof(addr)) {
    bzero(&addr, sizeof(addr));
}

/**
 * 指定ip和port
 * @param ip
 * @param port
 */
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {
    bzero(&addr, sizeof(addr));
}
