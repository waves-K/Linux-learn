#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

volatile int a = 1;
void sigcb(int signo)
{
  a = 0;
  printf("--------------\n");
}

int main()
{
  signal(SIGINT, sigcb);
  while(a){

  }
  return 0;
}
