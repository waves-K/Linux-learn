/* 实现一个最简单的 http 服务器端程序
 *    <html><body><h1>Hello ~~</h1></body></html>
 */ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Usage: ./httpserv ip port\n");
    }
    // 1. 实现 tcp 服务端程序
    //  a.创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0){
        perror("socket error");
        return -1;
    }
    //  b.绑定地址信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t len = sizeof(struct sockaddr_in);
    int ret;
    ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if(ret < 0){
        perror("bind error");
        return -1;
    }
    //  c.开始监听
    ret = listen(sockfd, 5);
    if(ret < 0){
        perror("listen error");
        return -1;
    }
    while(1){
        struct sockaddr_in cli_addr;
        int newfd = accept(sockfd, (struct sockaddr*)&cli_addr, &len);
        if(newfd < 0){
            perror("accept error");
            continue;
        }
        char header[1024] = {0};
        ret = recv(newfd, header, 1023, 0);
        if(ret <= 0){
            perror("recv error");
            close(newfd);
            continue;
        }
        printf("header:[%s]\n", header);

        char *body = "<html><body><h1>Hello ~~</h1></body></html>";
        char *first = "HTTP/1.1 200 OK\r\n";
        // char *first = "HTTP/1.1 302 Found\r\n";
        char head[1024] = {0};
        sprintf(head, "Content-Length: %d\r\n", strlen(body));
        // strcat(head, "Location: http://www.baidu.com/\r\n\r\n");
        send(newfd, first, strlen(first), 0);
        send(newfd, head, strlen(head), 0);
        send(newfd, body, strlen(body), 0);
        close(newfd);
    }
    close(sockfd);
    return 0;
}
