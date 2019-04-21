/* 多线程实现
 */

#include "tcp_com.h"
#include <pthread.h>

class TcpServer{
    private:
        TcpSocket sock;
    public:
        static void *thr_start(void *arg){
            TcpSocket *cli_sock = (TcpSocket*)arg;
            while(1){
                std::cout << "get a new link" << std::endl;
                char buff[1024] = {0};
                if(cli_sock->Recv(buff, 1023) < 0){
                    break;
                }

                printf("client say: %s\n", buff);
                memset(buff, 0x00, 1024);
                scanf("%s", buff);
                if(cli_sock->Send(buff, strlen(buff)) < 0){
                    break;
                }
            }
            cli_sock->Close();
            delete cli_sock;
            return NULL;
        }
        bool Worker(TcpSocket *sock){
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, thr_start, (void*)sock);
            if(ret != 0){
                printf("create worker error\n");
                return false;
            }
            pthread_detach(tid);
            return true;
        }
        bool Start(std::string &ip, uint16_t port){
            CHECK_RET(sock.Socket());
            CHECK_RET(sock.Bind(ip, port));
            CHECK_RET(sock.Listen());
            while(1){
                TcpSocket *cli_sock = new TcpSocket();
                CHECK_RET(sock.Accept(cli_sock));
                Worker(cli_sock);

                //std::cout << "get a new link" << std::endl;
                //char buff[1024] = {0};
                //cli_sock.Recv(buff, 1023);

                //printf("client say: %s\n", buff);
                //memset(buff, 0x00, 1024);
                //scanf("%s", buff);
                //cli_sock.Send(buff, strlen(buff));
            }
            CHECK_RET(sock.Close());
        }
};

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("usage: ./tcp_serv ip port\n");
        return -1;
    }
    uint16_t port = atoi(argv[2]);
    std::string ip = argv[1];
    TcpServer server;

    server.Start(ip, port);
    return 0;
}
