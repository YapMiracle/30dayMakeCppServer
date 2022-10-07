#include <iostream>
#include <strings.h>
#include <cctype>
#include <unistd.h>
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
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 5005);
    
    int b=bind(serv_sock, (struct sockaddr*)&serv_addr->addr, sizeof(serv_addr->addr));
    error_if(b==-1, "bind error");
    int l=listen(serv_sock, 5);
    error_if(l==-1, "listen error");
    char buf[READ_BUFFER];
    while(true) {

        InetAddress* client_addr = new InetAddress();
        int len = sizeof(client_addr->addr);
        int client_fd=accept(serv_sock, (struct sockaddr*)&client_addr->addr, (socklen_t *)&len);
        error_if(client_fd==-1, "accept error");
        int read_size = read(serv_sock, buf, sizeof(buf));
        error_if(read_size==-1, "read error");
        printf("buf=%s\n", buf);
        if(read_size==-1){
            break;
        }
    }
    close(serv_sock);
    return 0;
}
