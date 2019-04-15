/* 信号的阻塞演示
 *    sigprocmask
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

// signo 这个参数是操作系统赋予的，代表的是当前哪一个信号触发这个回调了
void sigcb(int signo){
  printf("recv signo:%d\n", signo);
}
int main()
{
  // int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
  //    how:    对 blocked 集合要做的操作
  //        SIG_BLOCK     讲 set 集合中的信号添加到 blocked 中
  //        SIG_UNBLOCK   讲 set 集合中的信号从 blocked 中移除
  //        SIG_SETMASK   使用 set 中信号替换 blocked 中的信号
  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = sigcb;
  // 清空信号集合
  sigemptyset(&action.sa_mask);

  sigaction(SIGINT, &action, NULL);
  sigaction(40, &action, NULL);

  sigset_t block;
  sigset_t old;
  sigemptyset(&block);
  // int sigfillset(sigset_t *set);
  // 向集合中添加所有信号
  // int sigaddset(sigset_t *set, int signum);
  // 向集合中添加指定的信号
  // int sigdelset(sigset_t *set, int signum);
  // 从集合中移除一个指定的信号
  sigfillset(&block);
  sigprocmask(SIG_BLOCK, &block, &old);

  // 在没有按回车之前，信号会一直被阻塞
  getchar();
  // int sigpending(sigset_t *set);
  //    获取未决信号
  sigset_t pending;
  sigpending(&pending);
  int i = 1;
  for(i = 1; i < 32; i++){
    // 判断信号集中是否有指定信号
    if(sigismember(&pending, i)){
      printf("1 ");
    }
    else{
      printf("0 ");
    }
  }
  fflush(stdout);
  // 在按了回车之后，信号会被解除阻塞
  sigprocmask(SIG_UNBLOCK, &block, NULL);
  // 还原阻塞信号
  // sigprocmask(SIG_SETMASK, &old, NULL);
  
  // sigprocmask(SIG_BLOCK, &set, &old);
  // int sigsuspend(const sitset_t *mask);
  //   临时使用 mask 集合替换 block 集合，并且陷入休眠，被没有阻塞的信号唤醒之后
  //   将 block 集合还原回去

  while(1){
    printf("----------\n");
    sleep(10);
  }
  return 0;
}
