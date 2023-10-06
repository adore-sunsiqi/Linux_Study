/*
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
    - 作用：给某个进程或者进程组pid，发送某个信号sig
    - 参数：
        - pid：
            - > 0：将信号发送给指定的进程pid
            - = 0：将信号发送给当前的进程组
            - = -1：将信号发送给每一个有权限接收这个信号的进程
            - < -1：将信号发送给该pid取反后的进程
        - sig：需要发送的信号的编号或者宏值
            - 0：不发送任何信号
    - 返回值：
        - 成功：0
        - 失败：-1

*/

/*
#include <signal.h>

int raise(int sig);
    - 作用：给当前进程发送信号
    -参数：
        - sig：信号
    -返回值：
        - 成功：0
        - 失败：非0
*/

/*
#include <stdlib.h>

void abort(void);
    - 作用：发送SIGABRT信号给当前进程，用于杀死当前进程。

*/

/*
kill(getpid(), SIGABRT)/raise(SIGABRT)/abort()
*/

#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main()
{
    pid_t pid = fork();
    if(pid == 0){
        int i = 0;
        for(i; i < 5; i++){
            printf("child process\n");
            sleep(5);
            raise(SIGINT);
        }
    }else if(pid > 0){
        printf("parent process\n");
        sleep(2);
        printf("kill child process now\n");
        kill(pid, SIGINT);
        wait(NULL);
    }
    return 0;
}