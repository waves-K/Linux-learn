/* 演示 core dump
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void func(){
  while(1);
}

int main()
{
  printf("-------\n");
  printf("-------\n");
  printf("-------\n");
  func();
  printf("-------\n");
  printf("-------\n");
  printf("-------\n");
  return 0;
}
