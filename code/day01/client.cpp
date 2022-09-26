/*
 * @Author: hzy
 * @Date: 2022-09-26 17:21
 * @LastEditTime: 2022-09-26 18.24
 * @LastEditors: hzy
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \30dayMakeCppServer\code\day01\client.cpp
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <netdb.h>

#define handle_error(msg)\
    do{perror(msg); exit(EXIT_FAILURE);}while(0)
int main(int argc, char* argv[]) {
    if(argc!=3){
        std::cout<<"Usage: ./client port\n";
        std::cout<<"Example: ./client 5005\n";
        return -1;
    }
    struct hostent *h;
    if((h=gethostbyname(argv[1]))==0){
        handle_error("Get host by name failed!!");
        return -1;
    }
    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_fd==-1){
        handle_error("Socket failed!!!");
        return -1;
    }
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(atoi(argv[2]));
    memcpy(&serv_addr.sin_addr, h->h_addr, h->h_length);
    if(connect(serv_fd, (sockaddr*)&serv_addr, sizeof(serv_addr))!=0){
        handle_error("Connect failed!!!");
        return -1;
    }
    char buffer[1024];
    for(int i=0; i<10; i++){
        sprintf(buffer, "You are a pig%d", i);
        if(send(serv_fd, buffer, sizeof(buffer), 0)<=0){
            handle_error("Send failed!!!");
            break;
        }
        printf("Send: %s\n", buffer);
        bzero(buffer, sizeof(buffer));
//        sprintf(buffer, "O")
        if(recv(serv_fd, buffer, sizeof(buffer), 0)<=0){
            handle_error("Recv failed!!!");
            break;
        }
        printf("Recv: %s\n", buffer);
    }
    close(serv_fd);
    return 0;
}
