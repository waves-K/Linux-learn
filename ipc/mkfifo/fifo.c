/* 命名管道的基本使用
 *    命名管道可用于同一机器上任意进程间通信
 *    int mkfifo(const char* pathname, mode_t mode)
 */

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/stat.h>

int main()
{
  // umask 目的是不计算权限
  umask(0);
  // int mkfifo(const char *pathname, mode_t mode)
  //  功能：创建一个命名管道文件，他只是创建，并不打开
  //        所以如果要使用命名管道文件，就必须自己打开
  //    pathname: 命名管道文件路径名
  //    mode:     权限
  //  返回值：成功返回 0     失败返回 -1
  
  // mkfifo 是一个库函数，而 perror 是用来打印系统调用的错误信息的
  int ret = mkfifo("./test.fifo", 0664);
  if(ret < 0){
    if(errno != EEXIST){
      perror("mkfifo error");
      return -1;
    }
  }

  // 开始操作命名管道文件
  //  1.打开文件
  int fd = open("./test.fifo", O_RDONLY);
  if(fd < 0){
    // 打开失败
    perror("open error");
    return -1;
  }

  // 打开成功
  printf("open fifo success!!\n");
  // 读取数据
  while(1){
    char buff[1024] = {0};
    // 这里传参 1023 是因为数组最后一位要存放 \0 字符串结束标志
    read(fd, buff, 1023);
    printf("read buff:[%s]\n", buff);
  }

  close(fd);
  return 0;
}
