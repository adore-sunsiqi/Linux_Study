//僵尸进程
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    //创建子进程
    pid_t pid = fork();

    //判断是父进程还是子进程
    if(pid > 0){
        //如果大于0，返回的是创建的子进程的PID，当前是父进程
        printf("pid : %d\n", pid);
        while(1){
            printf("parent process, pid : %d, ppid : %d\n", getpid(),getppid());
            sleep(1);
        }

    } else if(pid == 0){
        //当前是子进程
        printf("child process, pid : %d, ppid : %d\n", getpid(),getppid());
    } else if(pid == -1){
        perror("fork");
        printf("创建子进程失败");
    }

    for(int i = 0;i < 5; i++){
        printf("i : %d, pid : %d\n", i, getpid());
        //sleep(1);
    }

    return 0;
}
