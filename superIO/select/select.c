/* select 基本使用
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

int main(){
    // 定义一个监控可读集合
    fd_set rfds;

    // 1.清空集合
    // FD_ZERO(fd_set *set);
    FD_ZERO(&rfds);

    // 2.向集合中填加描述符
    // void FD_SET(int fd, fd_set *set);
    FD_SET(STDIN_FILENO, &rfds);

    // 3.开始监控
    int max_fd = STDIN_FILENO;
    // int select(int nfds, fd_set *readfds, fd_set *writefds,
    //                   fd_set *exceptfds, struct timeval *timeout);
    //  nfds:         最大的描述符+1
    //  readfds:      可读事件的监控集合
    //  writefds:     可写事件的监控集合
    //  exceptfds:    异常事件的监控集合
    //  timeout:      超时等待时间
    //      NULL      没有描述符就绪则永久阻塞等待
    //      struct timeval
    //          tv_sec    秒
    //          tv_usec   微秒
    //  返回值：
    //      1.小于0 出错
    //      2.等于0 超时
    //      3.大于0 就绪的描述符个数
    int ret = select(max_fd + 1, &rfds, NULL, NULL, NULL);
    if(ret < 0){
        perror("select error");
        return -1;
    }else if(ret == 0){
        printf("time out!!\n");
    }
    printf("have desc ok!!\n");
    int i = 0;
    for(i = 0; i < max_fd + 1; i++){
        // int FD_ISSET(int fd, fd_set *set);
        // 判断描述符是否存在于集合中
        if(!FD_ISSET(i, &rfds)){
            continue;
        }
        // 如果当前数字在集合中，意味着这个数字所对应的描述符就绪了
        char buf[1024] = {0};
        read(i, buf, 1023);
        printf("buf:%s", buf);
    }
    return 0;
}
