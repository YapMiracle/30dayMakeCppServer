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
	int con=connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    error_if(con == -1, "socket connect error");
    if(con>=0){
    	printf("Client is connected!!\n");
    }
    
    while(true){
        char buf[BUFFER_SIZE];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            close(sockfd);
            error_if(true, "socket read error");
        }
    }
    close(sockfd);
    close(sockfd);
    return 0;
}
