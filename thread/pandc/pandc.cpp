/* 生产者与消费者模型
 */

#include <iostream>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// 封装一个类
class BlockQueue{
    private:
        std::queue<int> _list; 
        int _cap;                 // 队列最大结点个数
        pthread_mutex_t _mutex;    // 互斥锁
        pthread_cond_t _productor;      // 条件变量
        pthread_cond_t _consumer;
        // 判断队列是否满
        bool QueueIsFull(){
            return (_list.size() == _cap);
        }
        // 判断队列是否为空
        bool QueueIsEmpty(){
            return (_list.size() == 0);
            //return _list.empty();
        }
    public:
        // 构造函数
        // 构造函数中是做初始化的，构造函数中并不是所有的初始化都放在里边，
        // 构造函数中一定要简单，放的是不会出错的东西，基本上不会出错的初始化才会放在里边
        // 因为构造函数我们没法判断返回值，一旦出错就只能断言退出
        BlockQueue(int cap = 10): _cap(cap){
            pthread_mutex_init(&_mutex, NULL);
            pthread_cond_init(&_productor, NULL);
            pthread_cond_init(&_consumer, NULL);
        }
        // 析构函数
        // 退出的时候调用，对象释放的时候调用
        ~BlockQueue(){
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_productor);
            pthread_cond_destroy(&_consumer);
        }
        // 入队
        bool QueuePush(int data){
            // 加锁，为了保证入队的安全性
            pthread_mutex_lock(&_mutex);
            while(QueueIsFull()){
                // 队满，不能入队，则等待
                pthread_cond_wait(&_productor, &_mutex);
            }
            _list.push(data);
            // 解锁
            pthread_mutex_unlock(&_mutex);
            // 唤醒
            pthread_cond_signal(&_consumer);
        }

        // 出队，获取数据
        bool QueuePop(int *data){
            pthread_mutex_lock(&_mutex);
            while(QueueIsEmpty()){
                // 队空，则等待
                pthread_cond_wait(&_consumer, &_mutex);
            }
            // 获取队列的头结点
            *data = _list.front();
            // 出队
            _list.pop();
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_productor);
        }
};

void *thr_consumer(void *arg){
    BlockQueue *q = (BlockQueue*)arg;
    while(1){
        int data;
        q->QueuePop(&data);
        printf("consumer get data:%d\n", data);
    }
    return NULL;
}

void *thr_productor(void *arg){
    BlockQueue *q = (BlockQueue*)arg;
    int i = 0;
    while(1){
        printf("productor put data:%d\n", i);
        q->QueuePush(i);
        i++;
        usleep(100000);
    }
    return NULL;
}


int main()
{
    BlockQueue q;
    int i, ret;
    pthread_t ctid[4], ptid[4]; // 线程 id 
    for(i = 0; i < 4; i++){
        ret = pthread_create(&ctid[i], NULL, thr_consumer, (void*)&q);
        if(ret != 0){
            printf("create thread error\n");
            return -1;
        }
        ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&q);
        if(ret != 0){
            printf("create thread error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++){
        pthread_join(ctid[i], NULL);
        pthread_join(ptid[i], NULL);
    }
    return 0;
}
