#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  int pid = fork();
  if(pid == 0){
    sleep(5);
    // exit 退出一个函数
    // 他是一个接口，需要一个头文件 <stdlib.h>
    exit(0);
  }
  // uint64_t 表示他是一个无符号长整形数字
  // 在64位操作系统上，是一个8个字节的数字
  uint64_t a = 0;
  while(1){
    a++;
    usleep(1000);
  }
  return 0;
}
