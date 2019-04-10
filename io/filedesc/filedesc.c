/*  文件描述符 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

int main()
{
//  close(1);

  int fd = open("./test.txt", O_RDWR);
  if(fd < 0){
    perror("open error");
    return -1;
  }

  // 将要打印的数据，不写入标准输出，而是写入test.txt
  dup2(fd, 1);

  printf("fd:%d\n", fd);    // stdout  ->   1 ---> test.txt
  // \n 换行刷新缓冲区 它仅仅针对标准输出文件才有效，即只对显示器生效
  // 其他文件, \n 仅仅具备换行效果
  fflush(stdout);

  // clode 是系统调用接口，关闭文件时不会刷新缓冲区，
  // (因为我们所说的这个缓冲区是 stdout 的缓冲区(用户态的缓冲区))
  // 文件描述符没有 用户态的缓冲区, 但实际上它的内核也会有缓冲的
  
  // 这个 close(fd) 关闭的系统调用接口，系统调用接口根本就没有缓冲区
  // 所以关闭的时候直接就关闭了，数据也就不会刷新到文件中去
  // 因为系统调用接口先将数据写入到缓冲区了，
  close(fd);
  return 0;
}
