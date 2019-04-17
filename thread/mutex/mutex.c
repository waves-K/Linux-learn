/* 一个基本的黄牛抢票程序
 *  其实就是一个变量来表示有多少票
 *  线程表示黄牛
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 使用全局变量定义票
// volatile 防止编译器过度优化
volatile int ticket = 100;
pthread_mutex_t mutex;
// 线程入口函数
void *thr_scalper(void *arg){
    int id = (int)arg;
    while(1){
        // 加锁，在对临界资源访问之前加锁
        // int pthread_mutex_lock(pthread_mutex_t *mutex);
        // int pthread_mutex_trylock(pthread_mutex_t *mutex);
        // lock -- 阻塞加锁
        // trylock -- 非阻塞加锁
        pthread_mutex_lock(&mutex);
        if(ticket > 0){
            usleep(100);  // 休息100微秒
            printf("scalper %d-%p get a ticket:%d\n", id, pthread_self(), ticket);
            ticket--;
        }else{
            // 在加锁之后任何有可能退出的地方都要解锁
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        // 解锁
        // int pthread_mutex_unlock(pthread_mutex_t *mutex);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t tid[4];
    int i, ret;

    // 初始化互斥锁，在创建线程之前进行初始化
    // int pthread_mutex_init(pthread_mutex_t *restrict mutex,
    //                const pthread_mutexattr_t *restrict attr);
    // pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    // mutex:    互斥锁变量
    // attr:     互斥锁属性，通常置NULL
    // 返回值：成功返回 0，   失败返回 error
    pthread_mutex_init(&mutex, NULL);
    for(i = 0; i < 4; i++){
        ret = pthread_create(&tid[i], NULL, thr_scalper, (void*)i);
        if(ret != 0){
            printf("create thread error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++){
        pthread_join(tid[i], NULL);
    }

    // 互斥锁销毁，必须在线程运行之后
    // int pthread_mutex_destroy(pthread_mutex_t *mutex);
    pthread_mutex_destroy(&mutex);
    return 0;
}
