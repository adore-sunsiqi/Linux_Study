// 死锁

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
        // pthread_mutex_lock(&mutex);// 重复加相同的锁，产生死锁

        if(tickets > 0){ // 临界区
            usleep(20000);
            printf("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
            tickets--; // 临界区
        }else{
            // 解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
        // 解锁
        pthread_mutex_unlock(&mutex);// 忘记解锁，产生死锁
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

/*
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 创建2个互斥量
pthread_mutex_t mutex1, mutex2;

void * workA(void * arg) {

    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex2);

    printf("workA....\n");

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}


void * workB(void * arg) {
    pthread_mutex_lock(&mutex2);
    sleep(1);
    pthread_mutex_lock(&mutex1);

    printf("workB....\n");

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

int main() {

    // 初始化互斥量
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // 创建2个子线程
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, workA, NULL);
    pthread_create(&tid2, NULL, workB, NULL);

    // 回收子线程资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
*/
