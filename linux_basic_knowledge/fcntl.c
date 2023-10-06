/*
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ...);
    作用：对文件描述符的操作
    参数：
        -fd：被操作的文件描述符，通过open获取
        -cmd：操作方式
            -F_DUPFD：复制fd的文件描述符，得到一个新的文件描述符
            -F_GETFL：获取指定的文件描述符的状态flag
            -F_SETFL：设置文件描述符的状态flag
                -必选项：O_RDONLY, O_WRONLY, or O_RDWR
                -可选项：O_APPEND：追加数据
                        O_NONBLOK：设置成非阻塞
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main()
{
    //修改或者获取文件状态
    int fd = open("a.txt", O_RDWR);
    if(fd == -1){
        perror("open");
        return -1;
    }

    //获取文件描述符的状态flag
    int flag = fcntl(fd, F_GETFL);
    flag |= O_APPEND;//加入新命令用：或 |。判断或修改用：与 &。

    //修改文件描述符状态flag，加入O_APPEND标记
    int fd1 = fcntl(fd, F_SETFL, flag);
    if(fd1 == -1){
        perror("fcntl");
        return -1;
    }

    char * str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}

