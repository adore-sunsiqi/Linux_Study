/*
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
    - 作用：设置定时器。函数调用，开始倒计时。
            当倒计时为0的时候，函数给当前进程发送一个SIGALARM信号
    - 参数：
        - seconds：倒计时的时长，单位秒。如果为0，则定时器无效。可以通过alarm(0)来取消一个定时器
    - 返回值：
        - 之前没有定时器，返回0
        - 之前有定时器，返回之前定时器的剩余的时间

    - SIGALARM：默认终止当前进程。每一个进程有且仅有唯一的一个定时器

    - alarm() 函数不阻塞!!!!

*/

// 定时器与进程的状态无关（自然定时法），无论进程处于什么状态，不会干扰定时。

#include <unistd.h>
#include <stdio.h>

int main()
{
    int seconds = alarm(3);
    printf("seconds = %d\n", seconds);// 0

    sleep(2);

    seconds = alarm(1);

    printf("seconds = %d\n", seconds);// 1

    while(1){
        
    }
    return 0;
}