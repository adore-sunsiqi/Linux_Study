/*
#include <pthread.h>

int pthread_cancel(pthread_t thread);
    - 作用：取消线程。让线程终止。（信号）
            不是立刻终止，而是子线程执行到一个取消点。才会终止
    - 参数：
        - thread：要取消的线程id
    - 返回值：
        成功：0
        失败：错误号
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int value = 10;

void * work(void * arg){
    printf("child thread...., id : %ld\n", pthread_self());
    sleep(3);
    //pthread_cancel(pthread_self());
    printf("嘿嘿 我还没退出\n");
}

int main()
{

    // 创建一个子线程
    pthread_t pthid;

  
    int ret = pthread_create(&pthid, NULL, work, NULL);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("errno: %s\n", errstr);
    }

    pthread_cancel(pthid);

    for(int i = 0; i < 5; i++){
        printf("i = %d\n", i);
    }
    
    printf("child thread id : %ld\n", pthid);
    printf("main thread id : %ld\n", pthread_self());

        

    pthread_exit(NULL);
    
    return 0;// 进程退出
}
