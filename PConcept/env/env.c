// 环境变量理解
// 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

// argc 命令行参数的个数
// argv 命令行参数具体内容
// env 当前进程对应的环境变量
// argv 和 env 这两个字符串数组的结构是非常相似的
// 都是在最后一个元素的后面有一个 NULL 指针作为标记
int main(int argc, char* argv[], char* env[]){
  (void)argc;
  (void)argv;
   int i = 0;
   //环境变量以空结尾
   for(; env[i] != NULL; ++i){
     printf("env[%d] = [%s]\n", i, env[i]);
   }
 
//  // extern 修改链接属性，声明一个变量
//  extern char** environ;
//   int i = 0;
//   //环境变量以空结尾
//   for(; env[i] != NULL; ++i){
//   printf("env[%d] = [%s]\n", i, environ[i]);
//   }
 

  char* ptr = getenv("SHELL");
  printf("PATH:[%s]\n", ptr);

#if 0
  // 根据 key 查找对应的 value
  char* result = getenv("HOME");
  printf("%s\n", result);
 
  // 使用 putenv 修改键值对
  putenv("HOME = /");
#endif 

  return 0;
}
