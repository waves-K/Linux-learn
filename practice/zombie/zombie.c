#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  pid_t ret = fork();
  if(ret < 0){
    // 创建进程失败
    perror("fork error");
    return -1;
  }
  else if(ret == 0){
    // child
    // 子进程三秒钟后退出
    printf("child[%d] is begin z...\n",getpid());
    sleep(3);
    exit(0);
  }
  else{
    // parent
    // 父进程睡眠 10 秒钟，然后再执行
    printf("parent[%d] is sleeping...\n", getpid());
    sleep(10);
  }
  return 0;
}
