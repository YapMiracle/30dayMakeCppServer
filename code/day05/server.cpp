#include <iostream>
#include <strings.h>
#include <cctype>
#include <unistd.h>
#include <cstring>
#include "Socket.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "util.h"

#define READ_BUFFER 1024
void handleReadEvent(int fd);

int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: ./server port.\n");
        return -1;
    }
    Socket* serv_sock = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 5005);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Epoll* epoll = new Epoll();
    serv_sock->setnonblocking();
    // 一个Channel负责一个Epoll和一个监听的fd
    Channel *servChannel = new Channel(epoll, serv_sock->getListen_fd());
//    epoll->addFd(serv_sock->getListen_fd(), EPOLLIN | EPOLLET);
    // 设置监听的fd的事件
    servChannel->enableReading();
    while(true){
        // 
        std::vector<Channel *> activeChannel = epoll->poll(0);
        int nfds = activeChannel.size();
        for(int i=0; i<nfds; i++){
            int chfd = activeChannel[i]->getFd();
            if(chfd == serv_sock->getListen_fd()) {
                InetAddress* client_addr = new InetAddress();
                Socket* client_sock = new Socket(serv_sock->accept(client_addr));
                client_sock->setnonblocking();
//                epoll->addFd(client_sock->getListen_fd(), EPOLLIN |EPOLLET);
                // 所有的channel都是使用一个ep
                Channel* clientChannel = new Channel(epoll, client_sock->getListen_fd());
                clientChannel->enableReading();
            } else if(chfd & EPOLLIN) {
                handleReadEvent(chfd);
            } else {
                printf("Something else happened\n");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}
void handleReadEvent(int fd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", fd, buf);
            for(int i=0; i<bytes_read; ++i){
                buf[i] = toupper(buf[i]);
            }
            write(fd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}
