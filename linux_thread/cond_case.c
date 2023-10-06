#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

void *producer(void *arg) {
    int item = 0;
    while (1) {
        pthread_mutex_lock(&mutex);
        
        // 缓冲区已满，等待消费者消费
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }
        
        buffer[count] = item;
        printf("Produced item: %d\n", item);
        count++;
        
        // 唤醒消费者线程
        pthread_cond_signal(&cond_consumer);
        
        pthread_mutex_unlock(&mutex);
        
        item++;
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        
        // 缓冲区为空，等待生产者生产
        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }
        
        int item = buffer[count - 1];
        printf("Consumed item: %d\n", item);
        count--;
        
        // 唤醒生产者线程
        pthread_cond_signal(&cond_producer);
        
        pthread_mutex_unlock(&mutex);
        
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    // 初始化互斥量和条件变量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);
    
    // 创建生产者线程和消费者线程
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    // 等待线程结束
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // 销毁互斥量和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);
    
    return 0;
}