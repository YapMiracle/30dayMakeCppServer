//
// Created by mirac on 2022/10/15.
//

#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>
#include <string.h>
#include <unistd.h>

#define READ_BUFFER 1024

/**
 * 初始化一个监听fd,
 * 和一个Acceptor,处理新连接事件
 * Acceptor的的回调是Server::newConnection
 * @param _loop
 */
Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr){
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

/**
 * 处理读事件
 * @param sockfd
 */
void Server::handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

/**
 * 处理连接事件
 * @param serv_sock
 */
void Server::newConnection(Socket *serv_sock){
    if(serv_sock->getFd()!=-1){
        Connection *conn = new Connection(loop, serv_sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[serv_sock->getFd()] = conn;
    }
}

void Server::deleteConnection(int sockfd) {
    if(sockfd!=-1){
        auto in = connections.find(sockfd);
        if(in!=connections.end()){
            Connection* conn = connections[sockfd];
            connections.erase(sockfd);
            close(sockfd);
            delete conn;
        }
    }
}

