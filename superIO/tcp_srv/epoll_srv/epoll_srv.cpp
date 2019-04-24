#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define EPOLL_MAX 100
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
class Epoll{
    private:
        // epoll 句柄--贯穿上下文
        int _epfd; 
        // 保存描述符与 TcpSocket 对应关系
        std::unordered_map<int, TcpSocket> _map;
    public:
        Epoll():_epfd(-1){}
        // 创建 epoll
        bool Create(){
            // int epoll_create(int size);
            // 创建 epoll 句柄，内核中创建事件集合
            //  size: 参数已经被忽略，赋值大于0即可
            _epfd = epoll_create(5);
            if(_epfd < 0){
                perror("epoll create error");
                return false;
            }
            return true;
        }
        // 向内核中的 epoll 结构中添加事件结点
        bool Add(TcpSocket &sock, uint32_t events = EPOLLIN){
            int fd = sock.GetFd();
            epoll_event ev;
            ev.data.fd = fd;
            ev.events = events;
            int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
            if(ret < 0){
                perror("epoll_ctl error");
                return false;
            }

            _map[fd] = sock;
            // int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
            //  epfd:     epoll 句柄
            //  op:       对 epoll 的操作
            //      EPOLL_CTL_ADD   添加事件
            //      EPOLL_CTL_MOD   修改事件
            //      EPOLL_CTL_DEL   移出事件
            //  fd:       要监控的描述符
            //  event：   监控描述符的对应事件信息
            //      struct epoll_event {
            //          uint32_t     events;      /* Epoll events */
            //          epoll_data_t data;        /* User data variable */
            //          typedef union epoll_data {
            //              void    *ptr;
            //              int       fd;
            //              uint32_t     u32;
            //              uint64_t     u64;
            //           } epoll_data_t;
            //      };
            //  返回值：
            //      成功：0     失败：-1
            return true;
        }
        // 从内核 epoll 结构中移除结点
        bool Del(TcpSocket &sock){
            int fd = sock.GetFd();
            // epoll_event ev;
            int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
            if(ret < 0){
                return false;
            }
            _map.erase(fd);
            return true;
        }
        // 开始监听，并且获取就绪 TcpSocket
        bool Wait(std::vector<TcpSocket> *list){
            // int epoll_wait(int epfd, struct epoll_event *events,
            //                       int maxevents, int timeout);
            //  epfd:       epoll 句柄
            //  events:     事件结构体数组(用于获取就绪事件)
            //  maxevents:  数组大小
            //  timeout:    epoll 阻塞超时时间(毫秒)  -1:永久阻塞
            //  返回值：
            //      < 0 : 出错
            //      ==0 ：超时
            //      > 0 : 就绪事件数量
            epoll_event evs[EPOLL_MAX];
            int nfds = epoll_wait(_epfd, evs, EPOLL_MAX, 3000);
            if(nfds < 0){
                perror("epoll_wait error");
                return false;
            }else if(nfds == 0){
                printf("epoll timeout!!\n");
                return false;
            }
            for(int i = 0; i < nfds; i++){
                list->push_back(_map[evs[i].data.fd]);
            }
            return true;
        }
};

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("./epoll_srv ip port\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());
    Epoll epoll;
    CHECK_RET(epoll.Create());
    CHECK_RET(epoll.Add(sock));
    while(1){
        std::vector<TcpSocket> list;
        if(epoll.Wait(&list) == false){
            continue;
        }
        for(int i = 0; i < list.size(); i++){
            if(list[i].GetFd() == sock.GetFd()){
                TcpSocket clisock;
                if(sock.Accept(&clisock) == false){
                    continue;
                }
                if(epoll.Add(clisock) == false){
                    clisock.Close();
                    continue;
                }
            }else{
                char buf[1024] = {0};
                if(list[i].Recv(buf, 1023) <= 0){
                    epoll.Del(list[i]);
                    list[i].Close();
                    continue;
                }
                printf("clisock say: %s\n", buf);
                list[i].Send("hello\n", 6);
            }
        }
    }
    return 0;
}
