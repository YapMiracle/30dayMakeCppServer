//
// Created by mirac on 2022/10/20.
//

#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

/**
 * 职责所在就是创建一个new Socket
 * 让后acceptChannel管理这个fd
 * 同时设置Channel的回调
 * Channel的回调是Acceptor::acceptConnection
 * @param _loop
 */
Acceptor::Acceptor(EventLoop* _loop) : loop(_loop){
    sock = new Socket();
    InetAddress* addr = new InetAddress("127.1", 5005);
    sock->bind(addr);
    sock->listen();
//    sock->setnonblocking();//set be ET
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
    acceptChannel->setUseThreadPool(false);
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void (Socket *)> _cb) {
    newConnectionCallback = _cb;
}

