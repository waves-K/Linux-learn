/* 管道符的基本实现---匿名管道 
 * ls -l | grep makefile
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/wait.h>
int main()
{
  int pid1, pid2;
  int pipefd[2];

  pipe(pipefd);
  if((pid1 = fork() == 0)){
    // ls -l  
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    execlp("ls", "ls", "-l", NULL);
  }
  if((pid2 = fork() == 0)){
    // grep makefile
    close(pipefd[1]);
    dup2(pipefd[0], 0);
    execlp("grep", "grep", "makefile", NULL);
  }
  close(pipefd[0]);
  close(pipefd[1]);
  
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  return 0;
}
 


