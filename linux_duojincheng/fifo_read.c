#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    //1.打开管道
    int fd = open("test", O_RDONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    //2、读数据
    while(1){
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(len == -1){
            perror("read");
            break;
        }else if(len == 0){
           printf("写端关闭断开连接了...\n");
           break;//使用exit会在close处产生警告
        }else{
            printf("recv data: %s\n", buf);
        }
    }
    close(fd);
    return 0;
}