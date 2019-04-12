/* 通过系统调用，获取到当前系统的进程 id (pid) */ 

// unistd.h 系统编程领域最重要的头文件，没有之一
// 里面包含了大部分的系统调用的 API 函数声明
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>

int main()
{
  pid_t ret = fork();
  if(ret < 0){      // 进程创建失败
    perror("fork");
    return -1;
  }
  else if(ret == 0){    // 这是子进程
    printf("I am child: %d ,ret: %d\n", getpid(), ret);
  }
  else{      // 这是父进程
    printf("I am parent: %d ,ret: %d\n", getppid(), ret);
  }
  sleep(1);
  return 0;
}
