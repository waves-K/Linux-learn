#include<fcntl.h>
#include <sys/wait.h>
#include<ctype.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  // 做一个界面出来
  while(1){
    // 等待标准输入
  
    // 此时不会立即打印到界面上，他只是写入到缓冲区中
    printf("[kbt@localhost]$ ");
    // 此时就会打印出来
    fflush(stdout);
    char buf[1024] = {0};
    // %[^\n]  正则表达式:表示从标准输入缓冲区中获取字符串，并不以空格结束，而是直到遇到 \n 为止
    // %*c / %*s 表示将一个 字符/字符串 从缓冲区中取出
    // 如果没有获取到字符 则重新获取
    if(scanf("%[^\n]%*c", buf) != 1){
      getchar();
      continue;
    }
    
    // 创建子进程---执行任务
    int pid = fork();
    if(pid < 0){
      continue;
    }
    else if(pid == 0){
      // 子进程---执行其他任务---程序替换
      // 解析是否有重定向符号
      //    >     >>
      char* ptr = buf;
      int redirect_flag = 0;    // 做一个重定向的标记，0 就代表没有重定向
      char* file;     // 文件名
      // ls -l >>      a.txt
      while(*ptr != '\0'){
        if(*ptr == '>'){
          *ptr++ = '\0';       // 对于我们的命令来说，是不能有 大于号 的，
          // 所以在解析的过程中如果遇到 > 就应该把这个位置替换为空
          // 代表命令从这里终止，这也就是为什么我们要把重定向写入到文件末尾的位置
          redirect_flag = 1;   // 代表有重定向了，并且他是一个清空重定向

          // 继续判断是否还有 > ,如果有就说明他是一个追加重定向
          if(*ptr == '>'){
            *ptr++ = 0;
            redirect_flag = 2; // 追加重定向
          }

          // 取文件名
          while(*ptr != '\0' && isspace(*ptr)){
            ptr++;
          }
          file = ptr;
          while(*ptr != '\0' && !isspace(*ptr)){
            ptr++;
          }
          *ptr = '\0';
        }
        ptr++;
      }

      int fd = -1;
      if(redirect_flag == 1){
        fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0664);
        dup2(fd, 1);
      }
      else if(redirect_flag == 2){
        fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0664);
        dup2(fd, 1);
      }
      // printf("buf:[%s]\n", buf);

      // 解析输入的数据
      // buf:[    ls    -a    ] 
      // [ls][-a]
      ptr = buf;
      char* argv[32];     // 定义参数数组
      int argc = 0;       // 定义参数个数
      while(*ptr != '\0'){
        if(!isspace(*ptr)){
          // 当前位置是非空白字符
          argv[argc++] = ptr;
          while(!isspace(*ptr) && *ptr != '\0'){
            ptr++;
          }
        }
        *ptr = '\0';
        ptr++;
      }
      // 这里是由于参数是以 NULL 结尾的
      argv[argc] = NULL;



      // 判断命令是否是内建命令
      if(strcmp(argv[0], "cd") == 0){
        // int chdir(const char* path);
        // 这是一个内建命令，改变当前工作路劲
        chdir(argv[1]);
        continue;
      }
      if(execvp(argv[0],argv) < 0){
        perror("");
      }
      // 替换失败就退出
      exit(0);
    }
    // 等待子进程退出-- 避免僵尸进程
    else{
      waitpid(pid, NULL, 0);
    } 

    // 如果打开文件，则进行关闭
    //int i = 0;
    //for(; i < argc; i++){
    //  printf("argv[%d]=[%s]\n", i, argv[i]);
    //}
  }
    return 0;
}
