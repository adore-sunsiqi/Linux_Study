/*
    标准C库函数
    #include <stdio.h>
    int fseek(FILE *stream, long offset, int whence)

    Linux系统函数
    #include <sys/types.h>
    #include <unistd.h>
    off_t lseek(int fd, off_t offset, int whence);
        参数：
            -fd：文档描述符
            -offset：偏移量
            -whence：指定的标记
                SEEK_SET：设置文件指针的偏移量，从文件头开始。
                SEEK_CUR：设置偏移量，当前位置+第二个参数的值。
                SEEK_END：设置偏移量，文件的大小+第二个参数的值。
        
        返回值：
            返回文件指针的位置

        作用：
            1.移动文件指针到头文件
                lseek(fd, 0, SEEK_SET)
            2.获取当前文件指针的位置
                lseek(fd, 0, SEEK_CUR)
            3.获取文件长度
                lseek(fd, 0, SEEK_END)
            4.拓展文件的长度，10b -> 110b：比如下载文件时，先用lseek函数占用磁盘空间，然后慢慢写入。
                lseek(fd, 100, SEEK_END)

*/

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    //打开文件
    int fd = open("lseek_test.txt", O_WRONLY);
    if(fd == -1){
        perror("open");
        return -1;
    }

    //拓展文件
    int ret = lseek(fd, 100, SEEK_END); 
    if(ret == -1){
        perror("open");
        return -1;
    }

    //写入空数据
    write(fd, " ", 1);//写入一个空字符

    //关闭文件
    close(fd);

    return 0;
}