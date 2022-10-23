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
    InetAddress *clnt_addr = new InetAddress();      //会发生内存泄露！没有delete
    Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));       //会发生内存泄露！没有delete
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getListen_fd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel *clntChannel = new Channel(loop, clnt_sock->getListen_fd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getListen_fd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}

void Server::deleteConnection(Socket *sock) {
    Connection * connection = connections[sock->getListen_fd()];
    connections.erase(sock->getListen_fd());
    delete connection;
    connection=nullptr;
}
