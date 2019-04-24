#include <iostream>
#include <string>
#include <unordered_map>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <vector>

class TcpSocket;
#define CHECK_RET(q) if((q) == false){return false;}
class TcpSocket{
    private:
        int _sockfd;
    public:
        TcpSocket():_sockfd(-1){}
        int GetFd(){
            return _sockfd;
        }
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
        ssize_t Send(const char *buf, size_t len){
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
class Select{
    private:
        // 最大的描述符
        int _max_fd;
        // 需要监控的集合
        fd_set _rfds;
        // 将描述符与socket进行关联，方便后边使用
        std::unordered_map<int, TcpSocket> _map;
    public:
        // 初始化
        Select():_max_fd(-1){
            // 清空集合
            // 因为我们不知道内存中有什么数据，为了避免他对我们的操作造成影响，所以将他进行清空
            FD_ZERO(&_rfds);
        }

        // 添加描述符 socket 
        bool Add(TcpSocket &sock){
            int fd = sock.GetFd();
            // 将描述符添加到 select 监控集合
            FD_SET(fd, &_rfds);
            // 在对象中添加描述符与TopSocket对应关系
            _map[fd] = sock;
            // 判断最大描述符
            _max_fd = fd > _max_fd ? fd : _max_fd;
            return true;
        }

        // 移除socket
        bool Del(TcpSocket &sock){
            // 1.从集合中移除
            // void FD_CLR(int fd, fd_set *set);
            //  从集合中移除指定的描述符
            int fd = sock.GetFd();
            FD_CLR(fd, &_rfds);

            // 2.从映射表中将它移除
            _map.erase(fd);

            // 3.从新获取最大的描述符
            // 遍历位图
            for(int i = _max_fd; i >= 0; i--){
                // 判断描述符是否在这个最大的描述符中
                if(FD_ISSET(i, &_rfds)){
                    _max_fd = i;
                }
            }
            return true;
        }
        bool Wait(std::vector<TcpSocket> *list, timeval *tv = NULL){
            int ret;

            fd_set set;
            memcpy(&set, &_rfds, sizeof(fd_set));
            ret = select(_max_fd + 1, &set, NULL, NULL, tv);
            if(ret < 0){
                perror("select error");
                return false;
            }else if(ret == 0){
                perror("select timeout!!\n");
                return false;
            }
            for(int i = 0; i < (_max_fd + 1); i++){
                if(!FD_ISSET(i, &set)){
                    continue;
                }
                list->push_back(_map[i]);
            }

            return true;
        }
};

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("./tcp_srv ip port\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    // 服务器的操作过程
    TcpSocket sock;
    Select s;
    
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());
    s.Add(sock);
    while(1){
        std::vector<TcpSocket> list;
        int ret = s.Wait(&list);
        if(ret == false){
            continue;
        }
        for(int i = 0; i < list.size(); i++){
            // 因为sock是监听socket，这个socket就绪，都是因为新的客户端
            // 连接请求到来
            if(list[i].GetFd() == sock.GetFd()){
                TcpSocket clisock;
                if(sock.Accept(&clisock) == false){
                    continue;
                }
                s.Add(clisock);

            }else{
                char buf[1024] = {0};
                if(list[i].Recv(buf, 1023) <= 0){
                    s.Del(list[i]);
                    list[i].Close();
                    continue;
                }
                list[i].Recv(buf, 1023);
                printf("client:%s\n", buf);
                if(list[i].Send("chishenme?\n", 11) < 0){
                    s.Del(list[i]);
                    list[i].Close();
                }
            }
        }
    }
    CHECK_RET(sock.Close());
    return 0;
}
