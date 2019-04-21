/* 多进程实现
 */

#include "tcp_com.h"
#include <sys/wait.h>
#include <signal.h>

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
        bool Worker(TcpSocket *cli_sock){
            int pid = fork();
            if(pid < 0){
            }else if(pid == 0){
                //if(fork() > 0){
                //    exit(0);
                //}
                while(1){
                std::cout << "get a new link" << std::endl;
                char buff[1024] = {0};
                cli_sock->Recv(buff, 1023);

                printf("client say: %s\n", buff);
                memset(buff, 0x00, 1024);
                scanf("%s", buff);
                cli_sock->Send(buff, strlen(buff));
                }
                cli_sock->Close();
                delete cli_sock;
                exit(0);
            }
           // else{
           //     waitpid(pid, NULL, 0);
           // }
            cli_sock->Close();
            delete cli_sock;
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

void sigcb(int signo){
    while(1){
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("usage: ./tcp_serv ip port\n");
        return -1;
    }
    signal(SIGCHLD, sigcb);
    uint16_t port = atoi(argv[2]);
    std::string ip = argv[1];
    TcpServer server;

    server.Start(ip, port);
    return 0;
}
