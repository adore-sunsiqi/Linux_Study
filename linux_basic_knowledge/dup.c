/*
#include <unistd.h>
int dup(int oldfd);
    作用：复制一个新的文件描述符
int dup2(int oldfd, int newfd);
    作用：重定向文件描述符
    参数：
        -oldfd：有效的文件描述符
        -newfd：自定义一个新的文件描述符
    

*/



#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main()
{
    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);
    int fd1 = dup(fd);//给a.txt文件一个新的文件描述符，
    //此时fd，fd1都指向a.txt文件
    //fd1默认选取空闲文件描述符中最小的

    if(fd1 == -1){
        perror("dup");
        return -1;
    }

    printf("fd : %d, fd1 : %d\n", fd, fd1);
    close(fd);

    char * str = "hello, world!";
    int ret = write(fd1, str, strlen(str));
    if(ret == -1){
        perror("write");
        return -1;
    }
    close(fd1);

    int fd_1 = open("b.txt", O_RDWR | O_CREAT, 0664);
    if(fd_1 == -1){
        perror("open");
        return -1;
    }

    int fd_2 = open("c.txt", O_RDWR | O_CREAT, 0664);
    if(fd_1 == -1){
        perror("open");
        return -1;
    }

    printf("fd_1 : %d, fd_2 : %d\n", fd_1, fd_2);

    int fd_3 = dup2(fd_1, fd_2);//fd_2将和fd_1指向同一个文件

    if(fd_3 == -1){
        perror("dup2");
        return -1;
    }

    char * str = "hello, world!";
    int ret_1 = write(fd_2, str, strlen(str));
    if(ret_1 == -1){
        perror("write");
        return -1;
    }

    printf("fd_1 : %d, fd_2 : %d, fd_3 : %d\n", fd_1, fd_2, fd_3);
    

    close(fd_1);
    close(fd_2);

    return 0;
}