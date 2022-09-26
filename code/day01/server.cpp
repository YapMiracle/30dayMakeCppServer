#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>

#define handle_error(msg)\
    do{perror(msg); exit(EXIT_FAILURE);}while(0)
int main(int argc, char* argv[]) {
    if(argc!=2){
        printf("Example: ./server 5005");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    bzero(serv_addr.sin_zero, 8);
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, SOMAXCONN);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);

//    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    char buffer[1024];
    while(true){
        bzero(buffer, sizeof(buffer));
        if(recv(clnt_sockfd, buffer, sizeof(buffer), 0)<=0){
            handle_error("Recv failed!!!");
            break;
        }
        printf("Recv: %s\n", buffer);
        sprintf(buffer, "Ip is %s\n", inet_ntoa(clnt_addr.sin_addr));
        if(send(clnt_sockfd, buffer, sizeof(buffer), 0)<=0){
            handle_error("Send failed");
            break;
        }
        printf("Send: %s\n", buffer);
        
    }
    close(clnt_sockfd);
    close(sockfd);
    return 0;
}
