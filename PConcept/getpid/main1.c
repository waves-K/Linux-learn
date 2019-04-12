#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

// 定义一个全局变量
// 定义全局变量时 最好在前边加上 g_ 这样便于在后边的代码中只要看到 g_ 就知道时全局变量
int g_count = 100;
int main(){
  pid_t ret = fork();
  if(ret > 0 ){
    // father
    sleep(1);
    printf("father: %d, %p\n", g_count, &g_count);
  }else if(ret == 0){
    // child
    g_count = 200;
    printf("child: %d, %p\n",g_count, &g_count);
  }
  return 0;
}
