/*
#include <signal.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
    - 作用：检查或者设置对信号的处理。信号捕捉
    - 参数：
        - signum：需要捕捉的信号的编号或者宏值
        - act：捕捉信号之后的处理动作
        - oldact：上次的设置，NULL
    - 返回值：
        成功：0
        失败：-1

struct sigaction {
               void     (*sa_handler)(int);// 函数指针，指向信号捕捉到之后的回调函数
               void     (*sa_sigaction)(int, siginfo_t *, void *);// 不常用
               sigset_t   sa_mask;// 临时阻塞信号集。在信号捕捉函数执行过程中临时阻塞某些信号。结束后就不再阻塞
               int        sa_flags;// 设置为0，采用sa_handler；设置为SA_SIGINFO，采用sa_sigaction
               void     (*sa_restorer)(void);// 被废弃掉。 NULL
           };


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
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);//清空临时阻塞信号集

    int ret_1 = sigaction(SIGALRM, &act, NULL);
    if(ret_1 == -1){
        perror("sigaction");
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

    while(1){
        getchar();//获取键盘录入. 该函数是阻塞的
    }
    
    
    return 0;
}