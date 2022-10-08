#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

#define BUFFER_SIZE 1024 

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    error_if(sockfd == -1, "socket create error");
	
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(5005);

    error_if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");
    
    while(true){
    	int iret;
        char buf[BUFFER_SIZE];
        bzero(buf, sizeof(buf));
        printf("Please input a string sended to server:");
        scanf("%s", buf);
        iret = send(sockfd, buf, sizeof(buf), 0);
        error_if(iret<=0, "send error");
        printf("Send: %s\n", buf);
        bzero(buf, sizeof(buf));
        iret = recv(sockfd, buf, sizeof(buf), 0);
        error_if(iret<=0, "recv error");
        printf("Recv: %s\n", buf);
        
    }
    close(sockfd);
    return 0;
}
