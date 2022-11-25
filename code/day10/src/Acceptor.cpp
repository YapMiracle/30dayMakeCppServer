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
    addr = new InetAddress("127.1", 5005);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor() {
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void (Socket *)> _cb) {
    newConnectionCallback = _cb;
}
