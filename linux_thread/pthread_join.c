/*
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
    - 作用：和一个终止的线程进行连接。用于对终止线程的资源回收
            - 该函数是阻塞函数
            - 一次只能回收一个子线程
            - 一般在主线程中去使用
    - 参数：
        - thread：要回收的子线程的id
        - retval：接收子线程退出时的返回值。不需要时，设置NULL
            为什么是二级指针？
                - 因为子线程的处理函数是一级指针

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
    //return NULL;
    //int value = 10;// 返回的是局部变量
    pthread_exit((void *)&value);
    // return (void *)&value
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



    for(int i = 0; i < 5; i++){
        printf("i = %d\n", i);
    }

    printf("child thread id : %ld\n", pthid);
    printf("main thread id : %ld\n", pthread_self());


    int * pthptr;
    //pthread_join(pthid, NULL);
    ret = pthread_join(pthid, (void **)&pthptr);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("errno: %s\n", errstr);
    }
    printf("exit data : %d\n", *pthptr);// 随机变量。因为num是局部变量。全局变量不存在问题

    pthread_exit(NULL);
    
    return 0;// 进程退出
}
