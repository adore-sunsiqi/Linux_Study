/*
#include <pthread.h>

int pthread_detach(pthread_t thread);
    - 作用： 分离一个子线程。标记为分离的子线程的资源会被系统自动回收
            同一个子线程不能多次分离
            不能通过pthread_join去连接一个已经分离的线程。报错
    - 参数：
        - thread：子线程的id
    - 返回值：
        成功：0
        错误：错误号

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

    pthread_detach(pthid);

    for(int i = 0; i < 5; i++){
        printf("i = %d\n", i);
    }

    printf("child thread id : %ld\n", pthid);
    printf("main thread id : %ld\n", pthread_self());


    

    pthread_exit(NULL);

    printf("主线程结束运行\n");// 不会被打印
    
    return 0;// 进程退出
}
