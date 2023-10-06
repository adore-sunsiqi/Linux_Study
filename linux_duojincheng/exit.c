/*
#include <stdlib.h>//standard C 
void exit(int status)

#include <unistd.h>//linux
void _exit(int status)

status：进程退出时的状态信息，父进程回收子进程的时候会用到
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("hello world\n");
    printf("love you");
    
    exit(0);//此处的值和return保持一致.
    //exit会打印完所有结果。因为\n会刷新IO缓冲区。
    //_exit(0);只能打印出hello world，因为会跳过IO刷新。

    return 0;
}


