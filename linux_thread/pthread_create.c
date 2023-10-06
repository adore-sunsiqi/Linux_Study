/*
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
        void *(*start_routine) (void *), void *arg);
    - 作用：创建一个子线程
    - 参数：
        - thread：传出参数，线程创建成功后，子线程的线程id会写入到该参数中
        - attr：需要设置的子线程的属性。一般使用默认，NULL
        - (*start_routine)：函数指针，子线程需要处理的逻辑代码，回调函数，由程序员编写
        - arg：给第三个参数使用，为第三个函数传参
    - 返回值：
        成功：0
        失败：错误号.和errno不同。不能通过perrno获取，
                需要使用：char * strerror(int errnum)。<string.h>
*/

// 一般情况下，main函数所在的线程称为主线程，其余的为子线程


// 由于pthread是第三方库，所以需要指定动态链接库的名称或者地址
// Compile and link with -pthread.
// gcc pthread_create.c -o create -l pthread
// gcc pthread_create.c -o create -pthread

/*
(指针变量指向的是地址)
* 运算符是间接引用运算符，解引用运算符。用于访问指针所指向的内存位置的值。
        它可以用于指针类型变量前面，表示获取该指针所指向的值。
& 运算符是取地址运算符，用于获取变量的内存地址。
        它可以用于变量前面，表示获取该变量的地址。
*/


#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void * work(void * arg){
    printf("child thread....\n");
    printf("arg value : %d\n", *(int *)arg);
    *(int *)arg = 20;
    return NULL;
}

int main()
{

    // 创建一个子线程
    pthread_t pthid;

    int  num = 10;
    printf("num_addr: %p\n", &num);
    int ret = pthread_create(&pthid, NULL, work, (void *)&num);
    //int ret = pthread_create(&pthid, NULL, work, NULL);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("errno: %s\n", errstr);
    }



    for(int i = 0; i < 5; i++){
        printf("i = %d\n", i);
    }

    sleep(2);// 解决主线程已经运行结束，子线程无法运行的问题
    printf("num_addr: %d\n", *&num);
    
    return 0;// 进程退出；子线程也退出
}

