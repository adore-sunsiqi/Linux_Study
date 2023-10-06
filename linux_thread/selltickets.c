// 使用多线程去实现卖票的案例

// 有3个窗口，一共100张票

// 出现线程同步问题。

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int tickets = 100;

void * selltickets(void * arg){
    while(tickets > 0){
        usleep(20000);
        printf("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
        tickets--;
    }
    return NULL;
}
int main()
{
    
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


    pthread_exit(NULL);

    return 0;
}
