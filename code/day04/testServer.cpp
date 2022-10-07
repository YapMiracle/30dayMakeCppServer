#include <iostream>
#include <strings.h>
#include <cctype>
#include <unistd.h>
#include "Socket.h"
#include "Epoll.h"
#include "InetAddress.h"

#define READ_BUFFER 1024

void handleReadEvent(int);
int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: ./server port ");
        return -1;
    }
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 5005);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    char buf[1024];
    while(true) {
        InetAddress* client_addr = new InetAddress();
        serv_sock->accept(client_addr);
        int read_size = read(serv_sock->getListen_fd(), buf, sizeof(buf));

        printf("read: %s", buf);
        if(read_size==-1){
            break;
        }
    }

    delete serv_addr;
    delete serv_sock;
    return 0;
}

void handleReadEvents(int fd){
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
