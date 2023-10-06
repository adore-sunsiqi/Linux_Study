/*
goat：
    使用有名管道实现进程间的聊天。（进程A和进程B一人一次的顺序交互）

步骤：
    1、创建两个管道（管道是单向的字节流的）
    2、进程A：
        -以只写的方式打开管道1
        -以只读的方式打开管道2
        -循环写读
            -获取键盘录入，fgets函数（scanf遇到换行就结束）
            -写管道1
            //使用父子进程进行读写剥离，就可以实现非顺序性交互
            -读管道2
    进程B：
        -以只读的方式打开管道1
        -以只写的方式打开管道2
        -循环读写
            -读管道1
            -获取键盘录入，fgets函数（scanf遇到换行就结束）
            -写管道2
            
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    //1、判断管道是否存在
    int ret = access("fifo_1", F_OK);
    if(ret == -1){
        printf("管道不存在，已创建管道fifo_1....\n");

        ret = mkfifo("fifo_1", 0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }
    
    ret = access("fifo_2", F_OK);
    if(ret == -1){
        printf("管道不存在, 已创建管道fifo_2....\n");

        ret = mkfifo("fifo_2", 0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }
    
    //2、以只写的方式打开管道fifo_1
    int fdw = open("fifo_1", O_WRONLY);
    if(fdw == -1){
        perror("open");
        exit(0);
    }
    printf("打开管道fifo_1成功, 等待写入...\n");

    //3、以只读的方式打开管道fifo_2
    int fdr = open("fifo_2", O_RDONLY);
    if(fdw == -1){
        perror("open");
        exit(0);
    }
    printf("打开管道fifo_2成功, 等待读取...\n");

    //4、循环的写读数据
    char bufw[128];
    char bufr[128];

    pid_t pid = fork();

    if(pid > 0){
        while(1){
            memset(bufw, 0, 128);
            //获取标准输入的数据
            fgets(bufw, 128, stdin);
            //写数据
            int ret = write(fdw, bufw, strlen(bufw));
            if(ret == -1){
                perror("write");
                exit(0);//break;只能退出一次循环
            }
        }
    }else if(pid == 0){
        while(1){
            //读数据
            memset(bufr, 0, 128);
            ret = read(fdr, bufr, 128);
            if(ret <= 0){
                perror("read");
                exit(0);
            }
            printf("buf: %s\n", bufr);

        }
    }


    close(fdr);
    close(fdw);

    return 0;
}

/*
//4、循环的写读数据
char buf[128];
while(1){
    memset(buf, 0, 128);
    //获取标准输入的数据
    fgets(buf, 128, stdin);
    //写数据
    int ret = write(fdw, buf, strlen(buf));
        if(ret == -1){
            perror("write");
            exit(0);//break;只能退出一次循环
        }
    //读数据
    memset(buf, 0, 128);
    ret = read(fdr, buf, 128);
    if(ret <= 0){
        perror("read");
        exit(0);
    }
    printf("buf: %s\n", buf);
}
*/
