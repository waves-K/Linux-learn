/* 实现一个非阻塞IO
 * fcntl
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

// 封装
// void setnonblock(int fd){
//     fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
// }

int main(){
    char buf[1024] = {0};

    // 非阻塞
    // int fcntl(int fd, int cmd, ... /* arg */ );
    //  fd:文件描述符
    //  cmd:
    //    F_GETFL   获取属性状态(通过返回值返回当前属性)
    //    F_SETFL   设置属性状态(替换原有属性为当前arg属性)
    //  arg:要设置的属性信息
    //    O_NONBLOCK   设置描述符为非阻塞
    //  返回值:
    //    F_GETFL   当前属性
    //    F_SETFL   成功：0   失败：-1
    
    // 这是一个替换式的设置属性，一旦设置，原有属性就没了，但这并不是我们想要的
    // 我们想要的时保留原有属性的情况下添加O_NONBLOCK
    // 因此我们就应该先获取原有属性，然后再设置属性为原有属性基础上添加O_NONBLOCK

    // 调用封装
    // setnonblock(STDIN_FILENO);

    // 获取原有属性
    int flag = fcntl(STDIN_FILENO, F_GETFL, 0);
    // 设置属性
    fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK);
    // 判断返回值
    //  1.大于0 读取数据成功，任务完成，无需阻塞
    //  2.等于0 没有读到数据，未完成任务 阻塞
    //  3.小于0 出错，
    while(read(STDIN_FILENO, buf, 1024) < 0){
        // 此时判断时什么原因在成读取数据失败
        //  1.errno == EINTR  代表当前的读取操做是阻塞的，
        //    但是阻塞的原因是在读取数据的过程中被信号打断，可被原谅，该读取操作可以继续
        //  2.errno == EAGAIN  代表当前缓冲区没有数据，描述符没有就绪
        //    所以需要等会在重新读取
        //  3.其他错误  不可被原谅的，需要直接退出，表示这个操作符不可以继续使用
        if(errno == EINTR || errno == EAGAIN){
            sleep(1);
            printf("stdin buffer have no data\n");
            continue;
        }
        return -1;
    }

    // 阻塞
    // read(STDIN_FILENO, buf, 1024);
    printf("buf:[%s]\n", buf);
    return 0;
}
