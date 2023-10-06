/*
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
    - 作用：初始化信号量
    - 参数：
        - sem：信号量变量的地址
        - pshared：0 用在线程；非 0 用在进程
        - value：记录信号量中的值
int sem_destroy(sem_t *sem);
    - 作用：释放资源
int sem_wait(sem_t *sem);
    - 作用：对信号量加锁，调用一次就对信号量中的值进行减 1，
        如果信号量中的值为 0，则阻塞。
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
int sem_post(sem_t *sem);
    - 作用：对信号量解锁，调用一次就对信号量中的值进行加 1，
        如果信号量中的值大于 0，则解除阻塞
int sem_getvalue(sem_t *sem, int *sval);
    - 作用：获取信号量中的值
*/

// 实现生产者之间互斥，消费者之间互斥，生产者和消费者之间的同步

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex;

// 申请两个信号量
sem_t psem, csem;

void *producer(void *arg) {
    int item = 0;
    while (1) {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        
        // 缓冲区已满，等待消费者消费
        if(count == BUFFER_SIZE) {
            
            pthread_mutex_unlock(&mutex);
            sem_post(&csem);

        }else{
            buffer[count] = item;
            printf("Produced item: %d\n", item);
            count++;
   
        
            pthread_mutex_unlock(&mutex);
            sem_post(&csem);
        
            item++;
        }
        
        
       
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        
        // 缓冲区为空，等待生产者生产
        if(count == 0) {
                
            pthread_mutex_unlock(&mutex);
            sem_post(&psem);
        }else{
            int item = buffer[count - 1];
            printf("Consumed item: %d\n", item);
            count--;
        
            pthread_mutex_unlock(&mutex);
            sem_post(&psem);
            }
        
        
    }
    return NULL;
}

int main() {
    
    
    // 初始化互斥量和信号量
    pthread_mutex_init(&mutex, NULL);
    
    sem_init(&psem, 0, 6); 
    sem_init(&csem, 0, 0);



    // 创建5个生产者 5个消费者
    pthread_t prod[5], cons[5];
    for(int i = 0; i < 5; i++){
        pthread_create(&prod[i], NULL, producer, NULL);
       
        pthread_create(&cons[i], NULL, consumer, NULL);
        
    }

    for(int i = 0; i < 5; i++){
        
        pthread_join(prod[i], NULL);
       
        pthread_join(cons[i], NULL);
    }
    
    
    // 销毁互斥量和信号量
    pthread_mutex_destroy(&mutex);
    
    sem_destroy(&psem);
    sem_destroy(&csem);
    
    return 0;
}