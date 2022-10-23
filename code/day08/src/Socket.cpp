//
// Created by mirac on 2022/10/4.
//

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "Socket.h"
#include "util.h"
#include "InetAddress.h"

/**
 * 调用socket
 */
Socket::Socket() : listen_fd(-1),client_fd(-1) {
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_if(listen_fd==-1, "socket create failed");
//    error_if(client_fd==-1, "socket init failed");
}

/**
 * 利用已经有的fd初始化
 * @param fd
 */
Socket::Socket(int fd) : listen_fd(fd) {
    error_if(fd == -1, "socket create error");
}

/**
 * bind一个InetAddress
 * @param addr
 */
void Socket::bind(InetAddress *addr) {
    error_if(::bind(listen_fd, (sockaddr*)&addr->addr, addr->addr_len)==-1, "socket bind failed");
}

/**
 * 监听属性自己的fd
 */
void Socket::listen() {
    error_if(::listen(listen_fd, SOMAXCONN) == -1, "listen error");
}

/**
 * 接受一个连接
 * @param addr
 * @return
 */
int Socket::accept(InetAddress *addr) {
    int client_fd = ::accept(listen_fd, (sockaddr*)&addr->addr, &addr->addr_len);
    error_if(client_fd == -1, "accept failed");
    return client_fd;
}

/**
 * 设置fd为非阻塞
 */
void Socket::setnonblocking() {
    fcntl(listen_fd, F_SETFL, fcntl(listen_fd, F_GETFL) | O_NONBLOCK);
}

/**
 * getListen_fd
 * @return
 */
int Socket::getListen_fd() {
    return listen_fd;
}

/**
 * getClient_fd
 * @return
 */
int Socket::getClient_fd() {
    return client_fd;
}

/**
 * close(fd)
 */
Socket::~Socket() {
    if(listen_fd != -1){
        close(listen_fd);
        listen_fd = -1;
    }
    if(client_fd != -1){
        close(client_fd);
        client_fd = -1;
    }
}