/* 捕捉 SIGCHLD 信号，处理子进程的退出，避免僵尸进程
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void sigcb(int no)
{
  // 为什么要循环非阻塞处理SIGCHLD信号
  // 因为SIGCHLD信号有可能丢失，因此需要循环非阻塞将退出的子进程全部处理
  while(waitpid(-1, NULL, WNOHANG) > 0){
  }
}
int main()
{
  signal(SIGCHLD, sigcb);
  if(fork() == 0){
    sleep(3);
    exit(0);
  }
  while(1){
    printf("------smoking------\n");
    sleep(3);
  }
  return 0;
}
