/* 回顾一个标准库IO接口 */

#include<stdio.h>
#include<string.h>

int main()
{

  // 定义一个文件流指针
  FILE* fp = NULL;
  // 这里需要知道 fopen 是一个库函数, perror 是打印系统调用错误的
  // 这就意味着，如果库函数出错了,我们用 perror / streror 代表它打印出来的错
  // 我们有时候会有点看不懂
  // 这是因为一个库函数当中有可能会调用很多个系统调用接口
  // 具体哪一个出错是不一定的
  fp = fopen("./tmp.txt", "w+");
  if(fp == NULL){
    perror("fopen error");
    return -1;
  }

  // 写入数据
  char buf[1024] = "nihaoa~~bite~~\n";
  // size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
  // size_t size: 块大小
  // size_t nmemb: 有几块
  fwrite(buf, strlen(buf), 1, fp);

  // int fseek(FILE *stream, long offset, int whence);
  // 对 fp 文件读写位置从 whence 开始偏移 offset 个字节
  //  whence: 
  //    SEEK_SET   从文件起始位置开始偏移  
  //    SEEK_CUR   从文件读写位置开始偏移
  //    SEEK_END   从文件末尾位置开始偏移
  //  返回值: 从文件起始位置到当前跳转位置的偏移量
  fseek(fp, 0, SEEK_SET);

  // size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
  memset(buf, 0x00, 1024);    // 初始化这块 buf 的内存。从 buf 的起始位置开始，往后1024的内存都清空为 0
  int ret = fread(buf, 1024, 1, fp);
  printf("ret:%d--buf:[%s]\n", ret, buf);

  fclose(fp);
  return 0;
}
