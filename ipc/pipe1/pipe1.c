/* 匿名管道接口的基本使用 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<error.h>
#include<string.h>



int main()
{
  // int pipe(int pipefd[2]);
  //   pipefd: 是一个输出型参数，用于获取管道的操作描述符
  //      pipefd[0]：用与从管道中读取数据
  //      pipefd[1]: 用于向管道中写入数据
  //  返回值：
  //      成功返回 0;  失败返回 -1
  int pipefd[2];
  int ret = pipe(pipefd);
  if(ret < 0){
    perror("pipe error");
    return -1;
  }
  // 由于匿名管道只能用于具有亲缘关系的进程间通信
  // 因为只有具有亲缘关系的进程间通信才能复制到描述符，才可以进行操作
  // 所以需要注意的是:管道必须创建于创建子进程之前
  // 如果是在创建子进程之后才创建管道，那么子进程根本就复制不到管道的描述符
  // 所以必须是先创建管道，在创建子进程
  
  int pid = fork();
  if(pid < 0){
    return -1;
  }
  else if(pid == 0){
    // 子进程---读取管道中的数据
    close(pipefd[1]);
    close(pipefd[0]);
//
//    int i = 2;
//    while(i-- > 0){
//      char buff[1024] = {0};
//      if(read(pipefd[0], buff, 1023) == 0){
//        printf("all write close!!\n");
//        break;
//      }
//        printf("buff:%s--%d-%d\n", buff, pipefd[0], pipefd[1]);
//    }
  }else{
    close(pipefd[0]);
    sleep(3);
    // 父进程---向管道中写入数据
    char* ptr = "hot!!! === sleep!";
    // 这里不使用 sizeof 来获取字符串的长度是因为sizeof获取的是 ptr 这个指针的长度
    // 在 64位 操作系统上一个指针的长度是八个字节，并不能把写入的数据全部获取到
    // 而我们需要的是写入字符串的实际字符串的长度，所以使用strlen
    write(pipefd[1], ptr, strlen(ptr));
    while(1){
      printf("----------\n");
      sleep(1);
    }
  }
    // 写完之后就会运行下来，此时不要忘记关闭文件，因为描述符会占一个位置
    // 但是如果进程退出了就可以不关闭，如果不是退出就一定要关闭，否则会造成资源泄露
    close(pipefd[0]);
    close(pipefd[1]);
  return 0;
}
