/*
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
    -作用：创建一个有名管道。
    -参数：
        -pathname：有名管道的路径
        -mode：文件的权限。和open的mode一致
    -返回值：
        -成功：0
        -失败：-1

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{   
    //判断管道是否存在
    int ret = access("fifo1", F_OK);
    if(ret == -1)
    {
        printf("管道不存在，已创建管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1){
        perror("mkfifo");
        exit(0);
        }
    }
    
    return 0;
}