//同fifo_read.c文件一起，用于实现不同进程间的通信，非父子进程

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//向管道中写数据
int main()
{   
    //1、判断管道是否存在
    int ret = access("test", F_OK);
    if(ret == -1)
    {
        printf("管道不存在，已创建管道\n");
        
        //2、创建管道文件
        ret = mkfifo("test", 0664);

        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }
    //3、打开管道文件
    int fd = open("test", O_WRONLY);//只写方式打开管道文件
    //如果read.c未执行，即读端全部关闭，则将在此阻塞。
    //如果不阻塞，将在write处收到sigpipe信号.
    
    if(fd == -1){
        perror("open");
        exit(0);
    }

    //4、写入数据
    for(int i = 0; i < 100; i++){
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        printf("writen data: %s\n", buf);
        int ret = write(fd, buf, strlen(buf));
        if(ret == -1){
            perror("write");
            break;
        }
        sleep(1);
    }
    close(fd);
    
    return 0;
}