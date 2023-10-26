// 封装线程池类

#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <cstdio>

template<typename T> // 线程池定义为模板类为了代码复用，参数T是任务类

class threadpool{
    public:
        threadpool(int thread_number = 8, int max_requests = 10000);
        
        ~threadpool();

        bool append(T * request);

    private:
        static void * worker(void * arg); // 如何在静态函数里访问类成员？ 把this作为参数传递到worker当中！
        // 静态成员函数不能访问类成员变量！！

        void run(); // 线程池从工作队列中取数据

    private:
        int m_thread_number; // 线程的数量

        pthread_t * m_threads; // 线程池数组，大小为m_thread_number

        int m_max_requests; // 请求队列中最多允许的等待处理的请求数量

        std::list<T *> m_workqueue; // 请求队列

        locker m_queuelocker; // 互斥锁

        sem m_queuestat; // 信号量用来判断是否有任务需要处理

        bool m_stop; // 是否结束线程
};

template<typename T>
threadpool<T>::threadpool(int thread_number, int max_requests) : 
    m_thread_number(thread_number), m_max_requests(max_requests),
    m_stop(false), m_threads(NULL){
        if((thread_number <= 0) || max_requests <= 0){
            throw std::exception();
        }
        m_threads = new pthread_t[m_thread_number];
        if(!m_threads){
            throw std::exception();
        }

        for(int i = 0; i < thread_number; ++i){
            printf("create the %dth process....\n", i);

            if(pthread_create(m_threads + i, NULL, worker, this) != 0){// 让静态函数可以访问类中成员
                // C++中回调函数必须是静态函数，C中是全局函数
                delete[] m_threads;
                throw std::exception();
            } 

            if(pthread_detach(m_threads[i])){
                delete[] m_threads;
                throw std::exception();
            }
        }
}

template<typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
    m_stop = true;
}


template<typename T>
bool threadpool<T>::append(T * request){

    m_queuelocker.lock();
    if(m_workqueue.size() > m_max_requests){
        m_queuelocker.unlock();
        return false;
    }

    m_workqueue.push_back(request);

    m_queuelocker.unlock();

    m_queuestat.post();

    return true;
}

template<typename T>
void * threadpool<T>::worker(void * arg){
    
    threadpool * pool = (threadpool *)arg;
    
    pool->run();

    return pool;

}


template<typename T>
void threadpool<T>::run(){

    while(!m_stop){

        m_queuestat.wait();

        m_queuelocker.lock();

        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }

        T * request = m_workqueue.front(); 

        m_workqueue.pop_front();

        m_queuelocker.unlock();

        if(!request){
            continue;
        }

        request->process();

    }

}



#endif