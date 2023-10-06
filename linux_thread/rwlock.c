/*
#include <pthread.h>

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
           const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

*/

// 读的操作要远远大于写的操作。比如数据库读写

// 8个线程操作同一个全局变量。3个线程不定时写，5个线程不定时读

// 用互斥锁的话 读操作是顺序执行的，不能并发的执行，使用读写锁改进

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int num = 10;

pthread_rwlock_t rwlock;

void * writeFunc(void * arg){
    while(1){
        // 加锁
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, wpthid : %ld, num : %d\n", pthread_self(), num);
        // 解锁
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
        
    }
    return NULL;
}

void * readFunc(void * arg){
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("++read, wpthid : %ld, num : %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

int main()
{
    // 初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);

    // 1、创建8个线程
    pthread_t wpthid[3], rpthid[5];

    for(int i = 0; i < 3; i++){
        pthread_create(&wpthid[i], NULL, writeFunc, NULL);
        pthread_detach(wpthid[i]);
    }
    for(int i = 0; i < 5; i++){
        pthread_create(&rpthid[i], NULL, readFunc, NULL);
        pthread_detach(rpthid[i]);
    }

    sleep(10);
    
    pthread_rwlock_destroy(&rwlock);

    
    // 3、退出主线程
    pthread_exit(NULL);

    return 0;

}