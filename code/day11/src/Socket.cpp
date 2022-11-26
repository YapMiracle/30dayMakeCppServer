//
// Created by mirac on 2022/10/4.
//

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include "Socket.h"
#include "util.h"
#include "InetAddress.h"

/**
 * 调用socket
 */
Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    error_if(fd == -1, "socket create failed");
}

/**
 * 利用已经有的fd初始化
 * @param fd
 */
Socket::Socket(int fd) : fd(fd) {
    error_if(fd == -1, "socket create error");
}

/**
 * bind一个InetAddress
 * @param addr
 */
void Socket::bind(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddr_len();
    error_if(::bind(fd, (sockaddr*)&addr, addr_len) == -1, "socket bind error");
    _addr->setInetAddr(addr, addr_len);
}

/**
 * 监听属性自己的fd
 */
void Socket::listen() {
    error_if(::listen(fd, SOMAXCONN) == -1, "listen error");
}

/**
 * 接受一个连接
 * @param addr
 * @return
 */
int Socket::accept(InetAddress *_addr) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    bzero(&addr, sizeof(addr));
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr, &addr_len);
    error_if(clnt_sockfd == -1, "socket accept error");
    _addr->setInetAddr(addr, addr_len);
    return clnt_sockfd;
}

/**
 * 设置fd为非阻塞
 */
void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::getFd() {
    return fd;
}

void Socket::connect(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->getAddr();
    int addr_len = _addr->getAddr_len();
    error_if(::connect(fd, (struct sockaddr*)&addr, addr_len)==-1, "socket connect error");
}

/**
 * close(fd)
 */
Socket::~Socket() {
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}
