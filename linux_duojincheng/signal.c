/*
#include <signal.h>

typedef void (*sighandler_t)(int);// 函数指针。返回值是 void
            // int类型的参数表示捕捉到的信号的值

//typedef void *sighandler_t(int);// 函数，返回值是 void*

sighandler_t signal(int signum, sighandler_t handler);
    - 作用：设置某个信号的捕捉行为
    - 参数：
        - signum：要捕捉的信号
        - handler：设置对捕捉信号的行为
            - SIG_IGN：忽略信号
            - SIG_DFL：使用信号的默认行为
            - 回调函数：内核调用，由程序员负责实现这个函数。
                        看函数指针的定义去选择函数的类型。
    - 返回值：
        成功：返回上一次注册的信号处理函数的地址，第一次调用返回NULL
        失败：返回SIG_ERR，设置错误号



SIGKILL 和 SIGSTOP 不能被捕捉，不能被忽略

 
*/


#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void myalarm(int num){
    printf("捕捉到的信号的编号: %d\n", num);
    printf("-------------\n");
}

int main()
{
    //注册信号捕捉函数
    //signal(SIGALRM, SIG_IGN);
    //signal(SIGALRM, SIG_DFL);
    __sighandler_t ret_1 = signal(SIGALRM, myalarm);
    if(ret_1 == SIG_ERR){
        perror("signal");
        exit(0);
    }
    
    struct itimerval new_value;

    //设置间隔时间
    new_value.it_interval.tv_sec = 2;// 间隔2秒
    new_value.it_interval.tv_usec = 0;

    //设置延迟时间
    new_value.it_value.tv_sec = 5;// 延迟3秒后执行定时。这时候就会发送信号了
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);// 非阻塞
    if(ret == -1){
        perror("setitimer");
        exit(0);
    }

    getchar();//获取键盘录入. 该函数是阻塞的
    
    return 0;
}