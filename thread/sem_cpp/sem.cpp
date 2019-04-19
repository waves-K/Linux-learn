/* 基于 c++ 中的 vector 容器实现线程安全的环形队列
 * vector -- 一个线性表（数组）
 */

#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

class RingQueue{
    private:
        std::vector<int> _list;
        int _cap;
        pthread_mutex_t _mutex;
        sem_t _space;       // 空闲空间计数
        sem_t _data;        // 数据计数
        int _pro_step;
        int _con_step;
    public:
        RingQueue(int cap = 10):_cap(cap), _list(cap), 
            _pro_step(0), _con_step(0){
            sem_init(&_space, 0, cap);
            sem_init(&_data, 0, 0);
            pthread_mutex_init(&_mutex, NULL);
        }
        ~RingQueue(){
            sem_destroy(&_space);
            sem_destroy(&_data);
        }
        bool QueuePush(int data){
            sem_wait(&_space);
            pthread_mutex_lock(&_mutex);
            _list[_pro_step] = data;
            _pro_step++;
            // _pro_step %= _cap;
            if (_pro_step == _cap) _pro_step = 0;
            pthread_mutex_unlock(&_mutex);
            sem_post(&_data);
            return true;
        }
        bool QueuePop(int *data){
            sem_wait(&_data);
            pthread_mutex_lock(&_mutex);
            *data = _list[_con_step];
            _con_step++;
            // _con_step %= _cap;
            if (_con_step == _cap) _con_step = 0;
            pthread_mutex_unlock(&_mutex);
            sem_post(&_space);
            return true;
        }
};

void *thr_con(void *arg){
    RingQueue *q = (RingQueue*)arg;
    while(1){
        int data;
        q->QueuePop(&data);
        printf("consumer get data:%d\n", data);
    }
    return NULL;
}

void *thr_pro(void *arg){
    RingQueue *q = (RingQueue*)arg;
    int i = 0;
    while(1){
        sleep(1);
        printf("Productor put data:%d\n", i);
        q->QueuePush(i++);
    }
    return NULL;
}
int main()
{
    pthread_t ctid[4], ptid[4];
    int i, ret;

    RingQueue q;

    for (i = 0; i < 4; i++){
        ret = pthread_create(&ctid[i], NULL, thr_con, (void*)&q);
        if (ret != 0){
            printf("create thread error\n");
            return -1;
        }
        ret = pthread_create(&ctid[i], NULL, thr_pro, (void*)&q);
        if (ret != 0){
            printf("create thread error\n");
            return -1;
        }
    }
    for (i = 0; i < 4; i++){
        pthread_join(ctid[i], NULL);
        pthread_join(ptid[i], NULL);
    }
    return 0;
}
