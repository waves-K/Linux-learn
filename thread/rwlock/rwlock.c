/* 读写锁的基本使用：
 *    设置写优先
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// 读写锁变量
pthread_rwlock_t rwlock;

void *thr_reader(void *arg){
    while(1){
        // int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
        // int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
        pthread_rwlock_rdlock(&rwlock);
        printf("thread reader -------\n");
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

void *thr_writer(void *arg){
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("thread writer ----\n");
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

int main()
{
    pthread_t rtid[4], wtid[4];
    int i, ret;

    // 定义读写锁属性变量
    pthread_rwlockattr_t attr;

    // int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
    pthread_rwlockattr_init(&attr);

    // int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr,
    //      int __pref);
    //  attr:   读写锁属性
    //  __pref: 优先级属性
    //      PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP    写优先
    //      PTHREAD_RWLOCK_PREFER_READER_NP   读优先
    //      PTHREAD_RWLOCK_PREFER_WRITER_NP   读优先
    //  读写锁默认读优先
    pthread_rwlockattr_setkind_np(&attr,
            PTHREAD_RWLOCK_PREFER_READER_NP);
    

    // 读写锁初始化
    // int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
    //               const pthread_rwlockattr_t *restrict attr);
    pthread_rwlock_init(&rwlock, &attr);

    // 销毁属性
    // int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
    pthread_rwlockattr_destroy(&attr);

    for(i = 0; i < 4; i++){
        ret = pthread_create(&rtid[i], NULL, thr_reader, NULL);
        if(ret != 0){
            printf("create thread error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++){
        ret = pthread_create(&rtid[i], NULL, thr_writer, NULL);
        if(ret != 0){
            printf("create thread error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++){
        pthread_join(rtid[i], NULL);
        pthread_join(wtid[i], NULL);
    }
    // int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    pthread_rwlock_destroy(&rwlock);
}
