/*
    实现 ps aux | grep xxx

    子进程：ps aux，子进程结束后，将数据发送给父进程
    父进程：获取数据，过滤

    1、创建pipe()
    2、子进程调用execlp()，执行ps aux 命令。execlp()函数将结果输出到当前终端
    3、子进程将标准输出stdout_fileno重定向到管道的写端。（父进程）dup2函数实现
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>


int main()
{
    //创建一个管道
    int pipefd[2];
    int ret = pipe(pipefd);

    if(ret == -1){
        perror("pipe");
        exit(0);
    }

    //创建一个进程
    pid_t pid = fork();

    if(pid == -1){
        perror("fork");
        exit(0);
    } else if(pid == 0){
        //child process
        close(pipefd[0]);
        //重定向文件描述符
        //执行ps aux

        dup2(pipefd[1], STDOUT_FILENO);//重定向
     
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);

    }else if (pid > 0){
        //parent process
        close(pipefd[1]);
        sleep(1);
        char buf[1024] = {0};
        int len = 0;
        while((len = read(pipefd[0], buf, sizeof(buf) - 1)) > 0){
            //-1字符串结束符
            printf("%s", buf);
            memset(buf, 0, 1024);
        }
        wait(NULL);
    }

    return 0;
}