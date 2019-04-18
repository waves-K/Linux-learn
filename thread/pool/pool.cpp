/* 基于 c++ 的线程池演示
 */

#include <iostream>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// 任务类
class MyTask{
    private:
        int _data;
    public:
        MyTask(int data): _data(data){}
        void SetData(int data){_data = data;}
        void Run(){
            srand(time(NULL));
            int s = rand() % 5;
            printf("thread %p get data %d sleep--%d\n", pthread_self(), _data, s);
            sleep(s);
        }
};

// 线程池类
class ThreadPool{
    private:
        std::queue<MyTask *> _list;
        size_t _cap;
        pthread_mutex_t _mutex;
        pthread_cond_t  _full;
        pthread_cond_t  _empty;

        // 线程池的最大线程数量
        int  _max_thr;

        bool QueueIsEmpty(){
            return (_list.size() == 0);
        }

        bool QueueIsFull(){
            return (_list.size() == _cap);
        }

        bool QueuePush(MyTask *t){
            _list.push(t);
        }

        bool QueuePop(MyTask **t){
            *t = _list.front();
            _list.pop();
        }

        // 实现入口函数
        static void *thr_start(void *arg){
            while(1){
                ThreadPool *p = (ThreadPool*)arg;
                pthread_mutex_lock(&p->_mutex);
                MyTask *task;
                while(p->QueueIsEmpty()){
                    // 没有任务，工作线程则等待
                    pthread_cond_wait(&p->_empty, &p->_mutex);
                }
                p->QueuePop(&task);
                pthread_mutex_unlock(&p->_mutex);
                pthread_cond_signal(&p->_full);
                task->Run();
                delete task;
            }
            return NULL;
        }
    public:
        ThreadPool(int thr = 5, int q = 10):_max_thr(thr), _cap(q){
            pthread_mutex_init(&_mutex, NULL);
            pthread_cond_init(&_full, NULL);
            pthread_cond_init(&_empty, NULL);
        }
        ~ThreadPool(){
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_full);
            pthread_cond_destroy(&_empty);
        }

        // 创建大量线程
        bool Init(){
            int ret;
            pthread_t tid;
            for(int i = 0; i < _max_thr; i++){
                ret = pthread_create(&tid, NULL, thr_start, (void*)this);
                if(ret != 0){
                    printf("create thread error\n");
                    return false;
                }
                // 分离线程
                pthread_detach(tid);
            }
            return true;
        }

        // 添加任务
        bool AddTask(MyTask *task){
            // 入队
            pthread_mutex_lock(&_mutex);
            while(QueueIsFull()){
                pthread_cond_wait(&_full, &_mutex);
            }
            QueuePush(task);
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_empty);
            return true;
        }
    
};

int main()
{
    ThreadPool p;

    MyTask *t;

    p.Init();
    int i = 0;
    while(1){
        printf("add task data:%d\n", i);
        t = new MyTask(i++);
        p.AddTask(t);
    }
    return 0;
}
