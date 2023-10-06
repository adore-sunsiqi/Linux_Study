/*
#include <signal.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
    - 作用：将自定义信号集中的信号设置到内核中。（设置阻塞、解除阻塞、替换）
    - 参数：
        - how：
            - SIG_BLOCK：将用户设置的阻塞信号集添加到内核中，内核中原来的数据不变。通过异或。mask |= set
            - SIG_UNBLOCK：解除阻塞。清除用户设置的数据对内核数据的干扰。mask &= ~set。然后去执行默认处理动作
            - SIG_SETMASK：用用户设置的数据去覆盖内核中的数据。
        - set：已经初始化好的用户自定义的信号集
        - oldset：保存上一次内核中阻塞信号集的状态。NULL。
    - 返回值：
        成功：0
        失败：-1


    int sigpending(sigset_t *set)
        - 作用：获取内核中的未决信号集
        - 参数： set。传出参数。保存内核中的未决信号集
*/


//编写一个程序，把所有的常规信号（1-31）的未决状态（1）打印到屏幕
//设置某些信号是阻塞的。通过键盘去产生未决状态

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{   
    //设置2、3号信号阻塞
    sigset_t set;
    sigemptyset(&set);
    //将2、3号信号添加到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);

    int num = 0;

    while(1){
        num++;
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        for(int i = 1; i <= 31; i++){
            if(sigismember(&pendingset, i) == 1){
                printf("1");
            }else if(sigismember(&pendingset, i) == 0){
                printf("0");
            }else{
                perror("sigismember");
                exit(0);
            }
        }
        printf("\n");
        sleep(1);

        if(num == 10){
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }

    }

    return 0;
}

//ctrl + z 可以停止
//后台运行  ./a.out &