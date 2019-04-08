#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  int pid = vfork();
  if(pid == 0){
    printf("child----%d\n", getpid());
    // exit(0);
  }
  else{
    printf("parent----%d\n", getpid());
  }
  return 0;
  while(1){
    printf("------%d\n",getpid());
    sleep(1);
  }
  return 0;
}
