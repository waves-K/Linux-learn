/* POSIX 标准信号量基本使用
 *    sem_init      初始化
 *    sem_destroy   信号量销毁
 *    sem_wait      信号量资源计数判断，大于0则返回，不大于0陷入休眠
 *    sem_timedwait   限时等待
 *    sem_post      唤醒等待队列上的信号量
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// 信号量变量
sem_t sem;

void *thr_pro(void *arg){
    while(1){
        sem_wait(&sem);
        sleep(1);
        printf("create noodle!!\n");
        // int sem_post(sem_t *sem);
        //  信号量计数 +1，并且唤醒等待队列中的pcb
        sem_post(&sem);
    }
    return NULL;
}

void *thr_con(void *arg){
    while(1){
        // int sem_wait(sem_t *sem);
        // int sem_trywait(sem_t *sem);
        // int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
        // 内部判断计数是否大于0
        //    大于0---立即正确返回，计数 -1
        //    不大于0---陷入等待
        sem_wait(&sem);
        printf("eat noodle!!\n");
        sem_post(&sem);
    }
    return NULL;
}


int main()
{
    pthread_t tid1, tid2;
    int ret;
    // int sem_init(sem_t *sem, int pshared, unsigned int value);
    //    sem:    信号量变量
    //    pshared:  应用范围
    //        0     线程间同步与互斥
    //        !0    进程间同步与互斥
    //    value:    信号量的初值
    sem_init(&sem, 0, 1);
    ret = pthread_create(&tid1, NULL, thr_pro, NULL);
    if (ret != 0) {
        printf("create error\n");
        return -1;
    }

    ret = pthread_create(&tid2, NULL, thr_con, NULL);
    if (ret != 0) {
        printf("create error\n");
        return -1;
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    // int sem_destroy(sem_t *sem);
    sem_destroy(&sem);
    return 0;
}
