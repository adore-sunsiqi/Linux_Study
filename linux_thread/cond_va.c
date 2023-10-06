// 生产者消费者模型

// 使用互斥锁和条件变量实现

// 实现了生产者之间互斥，消费者之间互斥，生产者消费者之间同步。

// 不考虑容器满的情况。使用链表


/*
#include <pthread.h>

pthread_cond_t cond

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
    - 作用：初始化
int pthread_cond_signal(pthread_cond_t *cond);
    - 作用：唤醒一个或多个阻塞的线程。但不能全部唤醒
int pthread_cond_broadcast(pthread_cond_t *cond);
    - 作用：唤醒全部阻塞的线程
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
    - 作用：等待，调用该函数，线程会阻塞
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,  const  struct  timespec
       *abstime);
    - 作用：设置等待时间
int pthread_cond_destroy(pthread_cond_t *cond);
    - 作用：释放条件变量

*/


#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


struct Node{
    int num;
    struct Node * next;
};

struct Node * head = NULL; // 头节点为空

pthread_cond_t cond;

pthread_mutex_t mutex;

void * product(void * arg){

    // 不断地创建新结点，添加到链表当中。采用没有头节点的头插法
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * newnode = (struct Node *)malloc(sizeof(struct Node));
        newnode->next = head;
        newnode->num = rand();
        head = newnode;
        printf("add node, num : %d, prod_pthid : %ld\n", newnode->num, pthread_self());
        
        // 只要生产了一个，就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    return NULL;
}

void * consumer(void * arg){
    
    // 不断地删除结点

    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * tmp = head;

        if(head != NULL){
            head = head->next;
            printf("del node, num : %d, cons_pthid : %ld\n", tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }else{
           pthread_cond_wait(&cond, &mutex);// 会先对mutex解锁
           pthread_mutex_unlock(&mutex);
        }
        
    }
    return NULL;
}


int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // 创建5个生产者 5个消费者
    pthread_t prod[5], cons[5];
    for(int i = 0; i < 5; i++){
        pthread_create(&prod[i], NULL, product, NULL);
        pthread_detach(prod[i]);
        pthread_create(&cons[i], NULL, consumer, NULL);
        pthread_detach(cons[i]);
    }


    while(1){
        sleep(40);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);



    return 0;
}