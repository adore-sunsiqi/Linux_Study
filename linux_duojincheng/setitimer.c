/*
#include <sys/time.h>

int getitimer(int which, struct itimerval *curr_value);
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
    - 作用：设定定时器，可以替代alarm函数，精度为微秒。可以实现周期计时。
    - 参数：
        - which：指定定时器以什么时间去计时
            - ITIMER_REAL：真实时间。时间到达，发送 SIGALRM信号给当前进程
            - ITIMER_VIRTUAL：用户态下进程消耗的CPU时间，包括该进程创建的所有线程的CPU消耗时间。时间到达，发送SIGVTALRM信号
            - ITIMER_PROF：该进程消耗的所有CPU时间。包括系统态和用户态的时间。发送SIGPROF信号。
        - new_value：设置定时器的属性。
        - old_value：记录上一次的参数。NULL
    - 返回值：
        - 成功：0
        - 失败：-1
*/


#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   
    struct itimerval new_value; 
    new_value.it_interval.tv_sec = 2;// 间隔2秒
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;// 延迟3秒后执行定时。这时候就会发送信号了。本程序在此停止
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);// 非阻塞
    if(ret == -1){
        perror("setitimer");
        exit(0);
    }

    getchar();//获取键盘录入.
    
    return 0;
}