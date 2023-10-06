/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *statbuf);
        作用：获取一个文件相关的信息
        参数：
            -pathname：操作的文件的路径
            -statbuf：结构体变量，传出参数，用于保存获取到的文件信息
        返回值：
            -成功：返回0；
            -失败：返回-1，并设置errno。

    int fstat(int fd, struct stat *statbuf);
    int lstat(const char *pathname, struct stat *statbuf);
        作用：获取软链接文件的信息(ln -s b.txt a.txt)b.txt->a.txt
        参数：
            -pathname：操作的文件的路径
            -statbuf：结构体变量，传出参数，用于保存获取到的文件信息
        返回值：
            -成功：返回0；
            -失败：返回-1，并设置errno。

*/


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    struct  stat statbuf;
    
    int ret = stat("cpy.txt", &statbuf);

    if(ret == -1){
        perror("open");
        return -1;
    }

    printf("size:%ld\n", statbuf.st_size);

    return 0;
}