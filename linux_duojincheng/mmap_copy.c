//使用内存映射实现文件拷贝的功能

/*
1、对原始文件进行内存映射
2、创建一个新文件，并扩展该文件、
3、把新文件的数据映射到内存
4、通过内存拷贝，将第一个文件的内容复制到第二个文件中
5、释放资源
*/

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fd = open("english.txt", O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    int size = lseek(fd, 0, SEEK_END);

    int fd_1 = open("new_english.txt", O_RDWR | O_CREAT, 0664);
    if(fd_1 == -1){
        perror("open");
        exit(0);
    }

    lseek(fd_1, size, SEEK_END);//需要执行一次写操作，大小大一个字节。
    write(fd_1, "", 1);

    //truncate("new_english.txt", size);//truncate()不需要写操作，大小一致


    void * ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void * ptr_1 = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_1, 0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    if(ptr_1 == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    memcpy(ptr_1, ptr, size);//内存拷贝函数

    munmap(ptr_1, size);
    munmap(ptr, size);
    
    close(fd_1);//为了避免依赖关系，先释放后创建的资源
    close(fd);
    

    return 0;
}