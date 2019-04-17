/* 演示线程的的创建
 * pthread_create
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg){
  pthread_t tid = pthread_self();

  while(1){
    printf("i am common thread!! %lu\n", tid);
    sleep(1);
  }
  return NULL;
}

int main()
{
  // int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  //                      void *(*start_routine) (void *), void *arg);
  // thread: 输出型参数，用于获取用户态线程 id 
  // attr:   线程属性，通常置 NULL
  // start_routine:  线程的入口函数
  // arg:    通过线程的入口函数，传递给线程的参数
  // 返回值：成功返回 0     失败返回 非0
  
  // 创建一个线程
  pthread_t tid;  // 线程 id


  int ret = pthread_create(&tid, NULL, thr_start, (void*)999);

  if(ret != 0){
    // 创建失败
    // 注意：
    //    pthread_create 是一个库函数，库函数尽量不要使用 perror 进行打印
    //    因为库函数内部系统调用与可能会调用很多，调用好几个系统调用
    //    那么这时候它报的错误我们就有可能看不懂
    //    所以还不如自己打印错误呢。
    printf("pthread create error\n");
    return -1;
  }

  // 线程创建成功后，这个进程中就有两个线程
  // 一个是主线程，另一个就是创建成功的那个普通线程
  // 接下来普通线程就会运行 thr_start 这个入口函数
  // 假如主线程运气比较好，直接拿到的时间片，他就会运行return 0
  // 在 main 函数中运行 return 0; 就会导致进程退出，进程推出就会释放资源
  // 此时所有的线程就会全部退出
  // 所以主线程一般情况下是不可以推出的，至少在main 函数中不能让他 return 0
  // 注意：
  //    线程之间没有父子之分，他只有主线程跟普通线程之分
  
  // %lu : 表示无符号的长整形
  printf("commom thread id:%lu\n", tid);
  while(1){
    printf("i am main thread!!\n");
    sleep(1);
  }
  return 0;
}
