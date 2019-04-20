/* 基于 udp 协议的简单网络通信程序
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建套接字
    // int socket(int domain, int type, int protocol);
    //    domain:     地址域
    //        AF_INET
    //    type:       套接字类型
    //        SOCK_STREAM   流式套接字，默认协议是TCP
    //        SOCK_DGRAM    数据报套接字，默认协议是UDP
    //    protocol:   
    //        IPPROTO_TCP   6
    //        IPPROTO_UDP   17
    //    返回值:   成功：socket 描述符(非负整数)   失败：-1
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0){
        perror("socket error");
        return -1;
    }
    // 2. 为套接字绑定地址信息
    // int bind(int sockfd, const struct sockaddr *addr,
    //                 socklen_t addrlen);
    //    sockfd:   套接字描述符
    //    addr:     地址信息
    //    addrlen:  地址信息长度
    //    返回值：  成功：0   失败：-1
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;

    // 主机字节序转换成网络字节序
    // uint16_t htons(uint16_t hostshort);
    //    2 个字节数据的主机字节序到网络字节序的转换
    addr.sin_port = htons(9000);
    // in_addr_t inet_addr(const char *cp);
    //    将点分十进制字符串 ip 地址转换为网络字节序 ip 地址
    addr.sin_addr.s_addr = inet_addr("172.17.106.20");
    // uint32_t htons(uint32_t hostlong);
    //    4 个字节数据的主机字节序到网络字节序的转换
    // addr.sin_addr.s_addr = htonl(0xc0a87a87); 
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if(ret < 0){
        perror("bind error");
        return -1;
    }
    while(1){
        // 3. 接受数据
        // ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
        //                    struct sockaddr *src_addr, socklen_t *addrlen);
        //    sockfd:     套接字描述符
        //    buf:        要接收的数据
        //    len:        要接收的数据长度
        //    flags:      0 - 默认阻塞
        //        MSG_PEEK    接收数据，但是并不将数据移出缓冲区
        //    src_addr:   数据的源地址信息
        //    addrlen:    源地址信息长度(输入输出复合型参数)
        //    返回值：  
        //        成功：实际接收到的数据长度
        //        失败：-1
        char buff[1024] = {0};
        struct sockaddr_in cli_addr;
        recvfrom(sockfd, buff, 1023, 0, (struct sockaddr*)&cli_addr, &len);
        printf("client say:%s\n", buff);

        // 4. 发送数据
        // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
        //                       const struct sockaddr *dest_addr, socklen_t addrlen);
        //    socket:     套接字描述符
        //    buff:       要发送的数据
        //    len:        要发送的数据长度
        //    flags:      0-默认阻塞
        //    dest_addr:  要发送的对端地址(客户端地址信息)
        //    返回值：
        //        成功：实际的发送长度
        //        失败：-1
        memset(buff, 0x00, 1024);
        scanf("%s", buff);
        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&cli_addr, len);
    }
    // 5. 关闭套接字
    close(sockfd);
    return 0;
}
