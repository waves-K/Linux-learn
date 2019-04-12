///////////////////////////////////////////////////////////
// 通过系统调用，获取到当前系统的进程 id (pid)
///////////////////////////////////////////////////////////
#include<stdio.h>
// unistd.h 系统编程领域最重要的头文件，没有之一
// 里面包含了大部分的系统调用的 API 函数声明
#include<unistd.h>

#if 0
int main()
{
  printf("hehe\n");
  pid_t pid = getpid();
  pid_t ppid = getppid();
  // 第一个 p 表示 parent
  // 第二个 p 表示 procrss
  printf("pid = %d, ppid = %d\n", pid,ppid);
  pid_t ret = fork();
  if(ret >0){
    // father
    printf("father: %d\n",getpid());
  }else if(ret == 0){
    // child
    printf("child: %d\n", getpid());
  }else{
  // failed
  perror("fork");
  }
  printf("ret = %d, pid = %d, ppid = %d\n", ret, getpid(), getppid());
  // sleep 休眠的时间单位是秒
  // usleep 休眠的时间单位是微妙
  sleep(1);
  while(1);
  return 0;
}
#endif

#if 0
int main(){
  while(1){
  sleep(1);
  }
  return 0;
}
#endif 


// 僵尸进程
int main(){
  pid_t ret = fork();
  if(ret > 0){
    // father
    printf("father: %d, %d\n", getpid(),getppid());
   // while(1){
    //  sleep(1);
   // }
  }else if(ret == 0){
   // child 
   printf("chlid: %d, %d\n", getpid(),getppid());
  }else{
    perror("fork");
  }
}
