/* 缓冲区的演示 */

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
  printf("%s", "printf");
  fprintf(stdout, "%s", "fprintf");
  fwrite("fwrite", 6, 1, stdout);

  write(1, "write", 5);

  sleep(3);
  return 0;
}
