// 生产者消费者模型

// 使用互斥锁实现

// 实现了生产者之间互斥，消费者之间互斥，生产者消费者之间互斥。没有实现生产者消费者之间的同步。

// 不考虑容器满的情况。使用链表


#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


struct Node{
    int num;
    struct Node * next;
};

struct Node * head = NULL; // 头节点为空

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
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    return NULL;
}

void * consumer(void * arg){
    
    // 不断地删除结点
    sleep(2);
    while(1){
        
        if(head != NULL){
            pthread_mutex_lock(&mutex);
            struct Node * tmp = head;
            head = head->next;
            printf("del node, num : %d, cons_pthid : %ld\n", tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }else{
            break;
        }
        
    }
    return NULL;
}


int main()
{
    pthread_mutex_init(&mutex, NULL);

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

    pthread_exit(NULL);



    return 0;
}

/*
1、ulimit -a 
    // 查看系统受限资源
2、ulimit -c unlimited 
    // 设置core不受限
3、cat /proc/sys/kernel/core_pattern 
    // 查看系统对core文件的处理。
    // 若显示 %p ... %E，说明系统对core进行了默认处理
4、sudo service apport stop 
    // 关闭系统支持服务。start 打开 restart 重启
5、gcc a.c -o a -g //
6、gdb core
7、core-file core
8、set breakpoint
*/