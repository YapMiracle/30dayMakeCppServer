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

int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: ./server port ");
        return -1;
    }
    Socket * serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 5005);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    char buf[READ_BUFFER];
    InetAddress* client_addr = new InetAddress();
    int client_fd=serv_sock->accept(client_addr);
    while(true) {
        int iret;
        bzero(buf, sizeof(buf));

        error_if(client_fd==-1, "accept error");
        iret=recv(client_fd,buf,sizeof(buf),0);
        error_if(iret<=0, "recv error");
        printf("Recv: %s\n", buf);
        strcpy(buf, "ok!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        iret=send(client_fd, buf, sizeof(buf), 0);
        error_if(iret<=0, "send error");
        printf("Send: %s\n", buf);
    }
    return 0;
}
