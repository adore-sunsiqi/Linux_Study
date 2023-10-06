//用SIGCHLD信号解决僵尸进程的问题

#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

void myFun(int num){
    printf("捕捉到的信号 : %d\n", num);
   //while(1){
   //   wait(NULL);
   //}

   while(1)
   {
    int ret = waitpid(-1, NULL, WNOHANG);
    if(ret > 0){
        printf("child die, pid = %d\n", ret);
    }else if(ret == 0){
        break;//说明还有子进程
    }else if(ret == -1){
        break;
    }
   }
}

int main()
{

    //出现段错误。子进程已经结束，父进程还没有注册完信号捕捉sigaction()
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);


    pid_t pid;
    for(int i = 1; i <= 20; i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }

    if(pid > 0){

        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        //注册完信号捕捉之后，释放阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        
        while(1){
            printf("parent process pid : %d\n", getpid());
            sleep(2);
        }
        

    }else if(pid == 0){
        
        printf("child process pid : %d\n", getpid());
    }

    return 0;
}