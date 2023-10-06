/*
// 互斥量类型：pthread_mutex_t

#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *restrict mutex,
           const pthread_mutexattr_t *restrict attr);
    - 作用：初始化互斥量
    - 参数：
        - mutex：需要初始化的互斥量变量
        - attr：互斥量相关的属性，默认NULL 
    - restrict：C语言的修饰符，被修饰的指针不能被另外的指针操作。


int pthread_mutex_destroy(pthread_mutex_t *mutex);
    - 作用：释放互斥量的资源

int pthread_mutex_lock(pthread_mutex_t *mutex);
    - 作用：加锁。阻塞。如果有一个线程加锁了，其他线程只能阻塞等待加锁

int pthread_mutex_trylock(pthread_mutex_t *mutex);
    - 作用：尝试加锁，如果加锁失败，不会阻塞，直接返回
int pthread_mutex_unlock(pthread_mutex_t *mutex);
    - 作用：解锁。

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 全局变量。所有线程共享。把共享变量设置为全局变量
int tickets = 100;

// 创建一个互斥量
pthread_mutex_t mutex;

void * selltickets(void * arg){
    
    // 卖票 
    while(1){
        // 加锁
        pthread_mutex_lock(&mutex);

        if(tickets > 0){// 临界区
            usleep(20000);
            printf("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
            tickets--;// 临界区
        }else{
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
        // 解锁
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
int main()
{
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 1. 创建三个子线程
    pthread_t pth1, pth2, pth3;
    pthread_create(&pth1, NULL, selltickets, NULL);
    pthread_create(&pth2, NULL, selltickets, NULL);
    pthread_create(&pth3, NULL, selltickets, NULL);

    // 2.回收子线程资源，阻塞函数
    pthread_join(pth1, NULL);
    pthread_join(pth2, NULL);
    pthread_join(pth3, NULL);

    // 2. 设置线程分离
    //pthread_detach(pth1);
    //pthread_detach(pth2);
    //pthread_detach(pth3);

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);


    pthread_exit(NULL);

    
    return 0;
}
