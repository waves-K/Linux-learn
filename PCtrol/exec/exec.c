/* exec 函数族中接口的使用 */
// 谁调用就对谁进行程序替换

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  printf("leihoua~~bite~~\n");
  // execl("/bin/ls", "ls", "-l", NULL);
  // execl("./test", "test", "-l", NULL);
  // execlp("ls", "ls", "-l", NULL);
  char* env[32];
  env[0] = "MYENV=1000";
  env[1] = NULL;
  // execle("./test", "test", "-a", NULL, env);
  printf("nihao~~bite~~\n");
  // 此时 nihao~~bite~~ 这个字符串是不会打印出来的
  // 指挥打印 leihoua~~bite~~ 这个字符串
  // 因为 execl("/bin/ls", "ls", "-l", NULL) 这一步进行替换之后
  // 当前这个进程的程序就变成了 ls ,也就不再是原来那个程序 exec 了
  // 所以代码中根本就不会有 后边这个 printf 语句了,自然就不会执行啦
  // 也就意味着 ls 这个程序运行完之后这个程序就结束了
  


  char* argv[32];
  argv[0] = "test";
  argv[1] = "kang";
  argv[2] = NULL;
  execve("./test", argv, env);
  printf("nihao~~bite~~\n");
  return 0;
}
