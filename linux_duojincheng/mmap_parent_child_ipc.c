//内存映射既可以实现父子进程之间的通信，也可以实现非关系进程之间的通信
//文件映射、匿名映射（匿名映射只能父子进程）

//内存映射通信是非阻塞的。


/*
#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    -作用：创建一个映射，映射文件到一块内存区域。
    -参数：
        -addr：内存映射的地址。设置为NULL，由内核指定。
        -length：要映射的数据的长度，必须大于0. 建议使用文件的长度。分页的整数倍
                获取文件的长度：stat、lseek
        -prot：对申请的内存映射区域的操作权限。PROT_READ | PROT_WRITE
            -PROT_EXEC：可执行权限
            -PROT_READ: 读权限。最基础的权限
            -PROT_WRITE：写权限
            -PROT_NONE：没有权限
        -flags：
            -MAP_SHARED：映射区域的数据会自动和磁盘文件进行同步。进程间通信，必须要有这个选项
            -MAP_PRIVATE：不同步，内存映射区域的数据改变，对原来的文件不会修改，会重新创建一个新的文件，进行写时复制
        -fd：需要映射的文件描述符，open得到。open的权限不能和prot冲突
        -offset：偏移量。必须是4K的整数倍。0表示不偏移
    -返回值：
        -成功：返回创建的内存映射的首地址。
        -失败：返回MAP_FAILED（void*（-1））

int munmap(void *addr, size_t length);
    -功能：释放内存映射
    -参数：
        -addr：要释放的内存的首地址。
        -length：要释放的内存映射区域的大小，和mmap的length一致。

*/

/*
1、如果对mmap的返回值 void * ptr 做 ++ 操作，ptr++，munmap是否能够成功？
    可以进行 ptr++ 操作，但munmap无法正确释放。

2、如果open 设置 O_RDONLY，mmap设置PROT_READ | PROT_WRITE会怎样？
    发生错误，返回 MAP_FAILED。（mmap: Permission denied）
    prot <= open

3、如果文件偏移量为1000会怎样？
    偏移量必须是4K的整数倍，返回MAP_FAILED

4、mmap什么情况下会调用失败？
    -length = 0
    -prot：只设置写权限；权限大于open的权限

5、open的时候O_CREAT一个新文件来创建映射区吗？
    可以，但是创建的文件的大小如果为0，会失败
    可以对新文件进行扩展
        -lseek()
        -truncate()

6、mmap后关闭文件描述符，对mmap映射有没有影响？
    没有影响，映射区域还在，创建映射区的fd关闭不会产生影响。（产生拷贝了）

7、对ptr越界操作会怎么样？
    越界操作操作的是非法内存，野内存，会产生段错误。
    
*/


/*
使用内存映射实现进程间通信
    1、有关系的进程通信
        -还没有子进程的时候，通过唯一的父进程，创建内存映射区域。
        -有了内存映射区域之后，创建子进程
        -父子进程共享创建的内存映射区域
    2、没有关系的进程通信
        -准备一个大小不为0的磁盘文件
        -进程A通过磁盘文件创建内存映射区域，得到一个操作内存映射区域的指针
        -进程B通过磁盘文件创建内存映射区域，得到一个操作内存映射区域的指针
        -使用内存映射区域进行通信。
*/



#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

int main()
{
    //1、打开一个文件
    int fd = open("mmap_ipc.txt", O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    //获取文件长度
    int size = lseek(fd, 0, SEEK_END);
    printf("size : %d\n", size);
    //2、创建内存映射区域
    void * ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    //printf("ptr : %s\n", (char *)ptr++);
    
    close(fd);

    //3、创建子进程
    pid_t pid =fork();
    if(pid > 0){
        wait(NULL);
        char buf[64];
        strcpy(buf, (char *)ptr); 
        printf("read data: %s\n", buf);
        
    }else if(pid == 0){
        //printf("ptr : %s\n", (char *)ptr);
        strcpy((char *)ptr, "nihaoya, my parent!!!");
    }else{
        perror("fork");
        exit(0);
    }

    int ret = munmap(ptr, size);
    if(ret == -1){
        perror("munmap");
        exit(0);
    }
    //close(fd);//在哪关闭都可以，不产生影响
    return 0;
}