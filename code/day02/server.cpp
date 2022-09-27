#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>

#define handle_error(msg)\
    do{perror(msg); exit(EXIT_FAILURE);}while(0)

#define error_if(condition, msg)\
    do{\
    if(condition){\
        perror(msg);\
        exit(EXIT_FAILURE);\
    }\
}while(0)
int main(int argc, char* argv[]) {
    if(argc!=2){
        printf("Example: ./server 5005");
        return -1;
    }
    int sockfd;
    error_if((sockfd= socket(AF_INET, SOCK_STREAM, 0))==-1, "Listen Socket created failed");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    bzero(serv_addr.sin_zero, 8);
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    error_if(listen(sockfd, SOMAXCONN)==-1, "Listen failed!!!");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd;
    error_if((clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len))==-1,"Accept failed");

//    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    char buffer[1024];
    while(true){
        bzero(buffer, sizeof(buffer));
        ssize_t read_bytes = read(clnt_sockfd, buffer, sizeof(buffer));
//        error_if(recv(clnt_sockfd, buffer, sizeof(buffer), 0)<=0, "Recv failed!!!");
        if(read_bytes > 0){
            printf("Message from client fd %d: %s\n", clnt_sockfd, buffer);
            write(clnt_sockfd, buffer, sizeof(buffer));
        } else if(read_bytes == 0){
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else if(read_bytes == -1){
            close(clnt_sockfd);
            error_if(true, "socket read error");
        }
        //bzero(buffer, sizeof(buffer));
        //scanf("%s", buffer);
        //ssize_t w = write(clnt_sockfd, buffer, sizeof(buffer));
       	//error_if(send(clnt_sockfd, buffer, sizeof(buffer), 0)<=0, "Send failed!!!");
        //if(w==-1){
        //    printf("socket already disconnected, can't write any more!\n");
        //    break;
        //}
//        printf("Recv: %s\n", buffer);
//        sprintf(buffer, "Ip is %s\n", inet_ntoa(clnt_addr.sin_addr));
//        error_if((send(clnt_sockfd, buffer, sizeof(buffer), 0)<=0),"Send failed!!!");
//        printf("Send: %s\n", buffer);

    }
    close(clnt_sockfd);
    close(sockfd);
    return 0;
}
