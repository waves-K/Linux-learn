/* 进程终止 demo */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int main()
{
  int pid = fork();
  if(pid < 0){
    perror("fork error");
    printf("fork error %s\n", strerror(errno));
  }
  // printf 打印一个数据,实际上他并没有把数据直接写入文件,而是先将数据写入缓冲区
  // 只有缓冲区写满了 或者刷新缓冲区的时候,才会将数据写入到文件当中.
  // 写入到标准输出就是打印，这里因为我们没有加 换行 也没有刷新缓冲区
  // 所以他会等到程序退出的时候才会刷新缓冲区，才打印出来
  printf("hello bit!!");
  int i = 0;
  // 为什么是小于 256 呢？
  // 我们每一个进程退出的返回值其实只用了一个字节来保存
  // 一个字节最大是 8 位，2^8 = 256 256是由于进位导致的，它的范围是 0 - 255
  //
  for(i = 0; i < 256; i++){
    printf("%s\n", strerror(i));
  }
  sleep(3);
  // 退出时不会刷新缓冲区，而是直接将资源释放，导致没有结果打印 
  _exit(256);
  // 也会在程序推出的时候刷新缓冲区，外在表现与 return 0 一样，但本质上还是有区别的
  exit(0);
  return 0;
}
