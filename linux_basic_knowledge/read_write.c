/*
    #include <unistd.h>
    ssize_t read(int fd, void *buf, size_t count);
        -fd：文件描述符，open得到的，通过文件描述符操作文件
        -buf：需要读取数据的存放的地方，数组的地址
        -count：指定数组的大小
    返回值：
        -成功：
            >0：返回实际的读到的字节数
            =0：文件读取完
        -失败：
            =-1：并且设置errno
    
    ssize_t write(int fd, const void *buf, size_t count);
        -fd：文件描述符，open得到的，通过文件描述符操作文件
        -buf：需要往磁盘写入的数据，数据
        -count：要写的数据的实际的大小
        返回值：
            -成功：返回写入的字节数
            -失败：-1，设置errno

*/

//文件拷贝

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    //1.通过open打开English.txt文件
    int srcfd = open("english.txt", O_RDONLY);
    if(srcfd == -1){
        perror("open");
        return -1;
    }

    //2.创建一个新的文件
    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0665);
    if(destfd == -1){
        perror("open");
        return -1;
    }

    //3.频繁的读写
    char buf[1024] = {0};
    int len = 0;
    while((len = read(srcfd, buf, sizeof(buf))) > 0){
        write(destfd, buf, len);
    }
    
    //4.关闭文件
    close(destfd);

    return 0;
}