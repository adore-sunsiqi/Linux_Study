/*
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);//creates a new process by duplicating（复制） the calling process
    作用：创建子进程
    返回值：
        fork()函数的返回值会返回两次，一次父进程，一次子进程
        在父进程中返回创建的子进程的PID；在子进程中返回0.
        如何区分父进程和子进程？通过fork()的返回值。
        返回-1：在父进程中返回，表示创建子进程失败，并且设置errno
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{

    int  num = 10;

    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //如果大于0，返回的是创建的子进程的PID，当前是父进程
        printf("pid : %d\n", pid);
        printf("parent process, pid : %d, ppid : %d\n", getpid(),getppid());

        printf("num: %d\n", num);
        num += 10;
        printf("num+=10 : %d\n", num);

    } else if(pid == 0){
        //当前是子进程
       

        printf("child process, pid : %d, ppid : %d\n", getpid(),getppid());
        
        printf("num: %d\n", num);
        num *= 10;
        printf("num*=10 : %d\n", num);
    } else if(pid == -1){
        perror("fork");
        printf("创建子进程失败");
    }

    for(int i = 0;i < 5; i++){
        printf("i : %d, pid : %d\n", i, getpid());
        sleep(1);
    }

    return 0;
}
