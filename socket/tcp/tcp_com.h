#ifndef __M_TCP_H__
#define __M_TCP_H__ 

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q) == false){return false;}

#endif

class TcpSocket{
    private:
        int _sockfd;
    public:
        TcpSocket():_sockfd(-1){}
        // 1. 创建 socket
        bool Socket(){
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(_sockfd < 0){
                perror("socket error");
                return false;
            }
            return true;
        }
        // 2. 绑定地址信息
        bool Bind(std::string &ip, uint16_t port){
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(sockaddr_in);
            int ret = bind(_sockfd, (sockaddr*)&addr, len);
            if(ret < 0){
                perror("bind error");
                return false;
            }
            return true;
        }
        // 3. 服务端-开始监听
        bool Listen(int backlog = 5){
            // int listen(int sockfd, int backlog);
            //    sockfd:   套接字描述符
            //    backlog:  最大的并发连接数
            //    返回值：  成功：0   失败: -1
            int ret = listen(_sockfd, backlog);
            if(ret < 0){
                perror("listen error");
                return false;
            }
            return true;
        }
        // 4. 服务端-获取连接成功的 socket 描述符
        bool Accept(TcpSocket *sock, sockaddr_in *_addr = NULL){
            // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
            //    sockfd:     套接字描述符
            //    addr:       新连接的客户端的地址信息
            //    addrlen:    地址信息长度(输入输出复合参数)
            //    返回值：
            //        成功: 建立的 socket 的描述符
            //        失败: -1
            int newfd;
            sockaddr_in addr;
            socklen_t len = sizeof(sockaddr_in);
            newfd = accept(_sockfd, (sockaddr*)&addr, &len);
            if(newfd < 0){
                perror("accept error");
                return false;
            }
            sock->_sockfd = newfd;
            if(_addr){
                memcpy((void*)_addr, (void*)&addr, len);
            }
            return true;
        }
        // 5. 服务端-收发数据
        ssize_t Recv(char *buf, size_t len){
            // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
            //    返回值：
            //        成功：返回实际的接收长度
            //        失败：-1
            ssize_t ret = recv(_sockfd, buf, len, 0);
            if(ret == 0){
                printf("peer shutdown!!\n");
                return 0;
            }
            return ret;
        }
        ssize_t Send(char *buf, size_t len){
            // ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            //    返回值：
            //      成功：实际的发送长度
            //      失败：-1
            int slen = 0;
            while(slen != 0){
                ssize_t s = send(_sockfd, buf + slen, len - slen, 0);
                if(s < 0){
                    if(errno == EINTR || errno == EAGAIN){
                        continue;
                    }
                    return -1;
                }
                slen += s;
            }
            return slen;
        }
        // 3. 客户端-发起连接请求
        bool Connect(std::string &ip, uint16_t port){
            // int connect(int sockfd, const struct sockaddr *addr,
            //                    socklen_t addrlen);
            //    sockfd:       套接字描述符
            //    addr:         服务端地址信息
            //    addrlen:      地址信息长度
            //    返回值：  成功：0   失败: -1
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(sockaddr_in);
            int ret = connect(_sockfd, (sockaddr*)&addr, len);
            if(ret < 0){
                perror("connect error");
                return true;
            }
            return true;
        }
        // 4. 客户端-收发数据
        //
        // 6. 关闭 socket 
        bool Close(){
            if(_sockfd >= 0){
                close(_sockfd);
                _sockfd = -1;
            }
            return true;
        }
};
