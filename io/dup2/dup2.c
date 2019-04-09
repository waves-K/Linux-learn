/* 演示文件重定向，与文件描述符分配规则
 */


#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>

int main()
{
  printf("stdin:%d\n", stdin->_fileno);
  printf("stdout:%d\n", stdout->_fileno);
  printf("stderr:%d\n", stderr->_fileno);

  // close(1);
  int fd = open("./tmp.txt", O_CREAT | O_RDWR, 0664);
  if(fd < 0){
    // perror : 打印上一条系统调用的错误信息
    perror("open error");
    return -1;
  }

  // int dup2(int oldfd, int newfd);
  //   文件重定向
  //   将 newfd 指向 oldfd 所指向的文件
  
  // 将 1 标准输出 重定向到 3 上，即重定向到 tmp.txt 上
  dup2(3, 1);

  printf("fd:[%d]\n", fd);
  close(fd);
  return 0;
}
