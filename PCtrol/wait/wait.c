#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  int pid = fork();
  if(pid < 0){
    perror("fork error");
    exit(-1);
  }
  else if(pid == 0){
    sleep(5);
    exit(0);
  }
  // pid_t wait(int *status);
  //  status: 是一个输出型参数，由于获取退出原因
  //  返回值: 退出的子进程的pid
  //int statu;
  //int ret = wait(&statu);
  //
  // pid_t waitpid(pid_t pid, int* status, int options);
  // pid:
  //    -1 : 等待任意子进程退出
  //    >0 : 等待指定子进程退出
  // status: 用于获取退出原因
  // options:
  //    WNOHANG  将 waitpid 设置成非阻塞
  // 返回值：
  //    -1：错误   0：没有子进程退出   >0：退出子进程的pid
  int statu;
  int ret;
  while((ret = waitpid(pid, &statu, WNOHANG)) == 0){
    printf("打麻将~~！\n");
    sleep(1);
  }
  // 获取返回值
  if(!(statu & 0x7f)){
    printf("%d-%d---child exit code:%d\n",ret, pid, (statu >> 8) & 0xff);
  }
  // WIFEXITED 是一个宏，是用来判断是否正常退出，如果是就返回真
  if(WIFEXITED(statu)){
    // WEXITSTATUS 通过 statu 获取到返回值
    printf("%d-%d---child exit code:%d\n",ret, pid, WEXITSTATUS(statu));
  }
  // 程序异常退出
  if(WIFSIGNALED(statu)){
    printf("exit signal:%d\n", WTERMSIG(statu));
  }
  while(1){
    sleep(1);
    printf("------\n");
  }
  return 0;
}
