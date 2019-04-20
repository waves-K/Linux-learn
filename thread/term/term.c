/* 线程的终止与等等
 *  return    pthread_exit    pthread_cancel
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg){
    char ptr[] = "test";
    // pthread_cancel((pthread_t)arg);
    // void pthread_exit(void *retval);
    //  线程退出，并返回一个数据
    // pthread_exit(NULL);
    pthread_exit("test");
    while(1){
        printf("i am child!!\n");
        sleep(1);
    }
    return "nihao";
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thr_start, NULL);
    if(ret != 0){
        printf("thread create error\n");
        return -1;
    }
    // int pthread_cancel(pthread_t thread);
    //  取消其他线程
    // pthread_cancel(tid);
    
    // 线程等待
    // int pthread_join(pthread_t thread, void **retval);
    //  等待指定的线程退出，并且获取返回值
    //  thread:   指定线程
    //  retval:   获取返回值
    
    char *ptr;
    // 线程被取消，则返回值不能作为标准结果，返回值是 -1
    pthread_cancle(tid);
    pthread_join(tid, (void**)&ptr);
    printf("child thread retval:%d\n",(int)ptr);
    while(1){
        printf("i am main\n");
        sleep(1);
    }

    return 0;
}
