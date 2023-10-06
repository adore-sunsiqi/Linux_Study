/*
#include <pthread.h>

void pthread_exit(void *retval);
    - 作用：终止一个线程。在哪个线程中调用就终止哪个线程。
            不会影响其他线程。（在主线程中调用不会影响子线程的运行）
            会立刻终止，不会再执行后续代码
    - 参数：
        - retval：传递指针。传出参数。可以在pthread_join()中获得
                    NULL。
*/

/*
pthread_t pthread_self(void)
    - 作用：获取当前线程的线程id。

int pthread_equal(pthread_t pth1, pthread_t pth2)
    - 作用： 比较两个线程的id是否相等
    不同的操作系统，pthread_t类型的实现不一样，有的是无符号长整型，有的是结构体
*/

/*
解决主线程已经运行结束，子线程还没有运行的问题？
    1、sleep(2)；// 不采用
    2、在主线程中调用pthread_exit函数

*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void * work(void * arg){
    printf("child thread...., id : %ld\n", pthread_self());
    
    return NULL;
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

    pthread_exit(NULL); // 不再执行后续代码，所以进程没有退出，子线程仍然可以执行
   
    
    return 0;// 进程退出
}
