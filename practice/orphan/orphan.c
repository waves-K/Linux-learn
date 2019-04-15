#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
  pid_t ret = fork();
  if(ret < 0){
    perror("fork error");
    return -1;
  }
  else if(ret == 0){
    // 让子进程睡眠 10 秒钟后开始执行
    printf("I am child~~, pid:%d\n", getpid());
    sleep(10);
  }
  else{
    // 父进程 3 秒钟后退出
    printf("I am parent ~~, pid:%d\n", getpid());
    sleep(3);
    exit(0);
  }
  return 0;
}
