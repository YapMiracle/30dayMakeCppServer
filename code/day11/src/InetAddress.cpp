//
// Created by mirac on 2022/10/4.
//

#include <cstring>
#include "InetAddress.h"

/**
 * 默认ip是INADDR_ANY，port是5005
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

sockaddr_in InetAddress::getAddr() {
    return addr;
}

void InetAddress::setInetAddr(sockaddr_in _addr, socklen_t _addr_len) {
    this->addr = _addr;
    this->addr_len = _addr_len;
}

socklen_t InetAddress::getAddr_len() {
    return addr_len;
}

InetAddress::~InetAddress() {
    bzero(&addr, sizeof(addr));
}