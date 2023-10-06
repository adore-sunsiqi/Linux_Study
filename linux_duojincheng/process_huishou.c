/*
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *wstatus);//父进程调用！！！
    -作用：等待任意一个子进程结束，结束后，该函数回收子进程资源
            一次回收一个进程。默认阻塞。只有当wait执行结束，才会执行wait之后的代码。
    -参数：
           进程退出时的状态信息。传入的是一个int类型的指针，是一个传出参数。（C通过指针；C++通过引用）
    -返回值：
        -成功：返回被回收的子进程的pid
        -失败：返回 -1，所有的子进程结束；调用函数失败
    调用wait函数的进程会被挂起（阻塞），直到他的；一个子进程退出
    或收到一个不能被忽略的信号。  没有子进程或子进程都结束则返回-1。   

pid_t waitpid(pid_t pid, int *wstatus, int options);
    -作用：回收指定的子进程。
    一次回收一个进程。可以设置为非阻塞，即不需要等待子进程执行结束就可以接着执行下面的代码
    -参数：
        -pid：
            < -1：回收某个进程组的组id的绝对值中的某个子进程.
            -1：回收任意一个子进程，相当于wait()，最常用
            0：回收当前进程组的任意一个子进程。进程组的pgid和创建该进程组的当前进程的pid相同
            > 0：子进程的pid。
        -*wstatus:
        -options:设置阻塞非阻塞
            -0：阻塞
            -WNOHANG：非阻塞
    -返回值：
        > 0：返回子进程的pid
        = 0：非阻塞时，表示还有子进程
        = -1：调用失败，没有子进程。


*/


#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*
int main()
{
    //有一个父进程，创建5个子进程
    pid_t pid;
    

    for(int i=0; i < 5; i++){
        pid = fork();
        if(pid == 0){//子进程不需要循环创建子进程了
            break;
        }
        //printf("i = %d\n", i);
        //printf("pid: %d, ppid: %d\n", pid, getppid());
    }

    if(pid > 0){
        //父进程
         printf("parent, pid = %d\n", getpid());
        while(1){
           
            int st;
            int ret = wait(&st);
            //int ret = wait(NULL);//NULL说明不需要传入子进程结束的状态
            //没有子进程结束的时候，会在此阻塞。
            if(ret == -1){
                break;
            }
            if(WIFEXITED(st)){
                //是否正常退出
                printf("退出状态码：%d\n", WEXITSTATUS(st));
            }
            if(WIFSIGNALED(st)){
                //是不是异常终止
                printf("被哪个信号干掉了：%d\n", WTERMSIG(st));
            }
            printf("child die, pid + %d\n", ret);
            sleep(1);
        }
        
    }else if(pid == 0){
        //子进程
        //while(1){
            printf("child, pid = %d\n", getpid());
            sleep(1);
       // }
        exit(0);
    }

    
    return 0;
}
*/
int main()
{
    //有一个父进程，创建5个子进程
    pid_t pid;
    

    for(int i=0; i < 5; i++){
        pid = fork();
        if(pid == 0){//子进程不需要循环创建子进程了
            break;
        }
        //printf("i = %d\n", i);
        //printf("pid: %d, ppid: %d\n", pid, getppid());
    }

    if(pid > 0){
        //父进程
         printf("parent, pid = %d\n", getpid());
        while(1){
           
            int st;
            int ret = waitpid(-1, &st, WNOHANG);
            if(ret == -1){
                break;
            }else if(ret == 0){
                continue;
            }else if(ret > 0){
                if(WIFEXITED(st)){
                //是否正常退出
                printf("退出状态码：%d\n", WEXITSTATUS(st));
                }
                if(WIFSIGNALED(st)){
                //是不是异常终止
                printf("被哪个信号干掉了：%d\n", WTERMSIG(st));
                }
                printf("child die, pid + %d\n", ret);
            }
            sleep(1);
        }
        
    }else if(pid == 0){
        //子进程
        while(1){
            printf("child, pid = %d\n", getpid());
            sleep(1);
        }
        exit(0);
    }

    
    return 0;
}

