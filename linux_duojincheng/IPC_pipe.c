/*
#include <unistd.h>
int pipe(int pipefd[2]);
    -作用：创建一个匿名管道，用于父子、兄弟进程间通信。
    -参数：int pipefd[2] 传出参数。
        pipefd[0]:管道的读取端
        pipefd[1]:写入端
    -返回值：
        成功：0
        失败：-1
*/
//默认是阻塞的。管道没有数据，读阻塞；管道满，写阻塞。

//子进程发送数据给父进程，父进程读取管道数据并输出
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    //在fork函数之前创建管道：对应同一个文件描述符。
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1){
        perror("pipe");
        exit(0);
    }

    //查看缓冲器大小
    long size = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("size : %ld\n", size);


    //创建子进程
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        exit(0);
    }else if(pid == 0){
        printf("i am child process, pid : %d\n", getpid());
        //关闭读端
        close(pipefd[0]);
              
        while(1){

            //写入数据
            char * str = "hello, i am child process";
            write(pipefd[1], str, strlen(str));
            sleep(5);
        }
        
    }else if(pid > 0){
        printf("i am parent process, pid : %d\n", getpid());
        close(pipefd[1]);

        //设置读非阻塞
        int flags = fcntl(pipefd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(pipefd[0], F_SETFL, flags);

        char buf[1024] = {0};
        while(1){
            //读取数据
            int len = read(pipefd[0], buf, sizeof(buf));
            if(len == -1){
            perror("read");
            }
            printf("len : %d\n", len);
            printf("parent recv: %s, pid : %d\n", buf, getpid());
            memset(buf, 0, 1024);
            sleep(1);
        }
        
    }
    return 0;
}