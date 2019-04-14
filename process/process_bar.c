#include<stdio.h>
// unistd.h 头文件是 Linux 系统编程中最重要的头文件
// 没有之一，包含了操作系统对程序员提供的 API(应用程序编程接口)
#include<unistd.h>

int main(){
  char split[4] = {'-', '/', '|', '\\'};
  char buf[1024] = {0};
  int i = 0;
  for(; i < 100; ++i){
    buf[i] = '#';
    //  \r  回车符，功能：让光标回到该行的起始位置。
    //  和 \n 相比, \n 会另起一个新行；\r 不会另起新行
    //  printf 确实执行到了。没有打印结果的原因是
    //  数据当前还在缓冲区中
    printf("[%d%%][%c][%s]\r",i, split[i % 4], buf);
    fflush(stdout);
    // usleep 让程序休眠，时间单位为 微秒
    usleep(200*1000);
  }
  return 0;
}
