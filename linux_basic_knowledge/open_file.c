/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{   //不是C++函数重载，是可变参数。

    //打开一个已经存在的文件
    int open(const char *pathname, int flags);
        参数：
            -pathname：要打开的文件路径
            -flags：文件权限的设置等。
            O_RDONLY, O_WRONLY, or O_RDWR.互斥，
        返回值：返回一个新的文件描述符，如果返回-1，则调用失败。
        errno：属于Linux系统函数库的一个全局变量，记录最近的错误号。

        perror：标准C库中的函数（man 3 perror）。
        #include <stdio.h>
        void perror(const char *s);
            s参数：用户描述
        作用：打印errno对应的错误描述（我们要的）。 



    //创建一个新文件
    int open(const char *pathname, int flags, mode_t mode);
        参数：
            -pathname：要创建的文件的路径
            -flags：文件权限设置。
                -必选项：O_RDONLY, O_WRONLY, or O_RDWR.互斥，
                -可选项：O_CREAT 文件不存在则创建新文件。
            -mode：八进制的数，表示创建的新文件的操作权限。如0777.
                最终的操作权限：mode & ~umask。
                umask：0002。~umask：0775
                0777   ->    111111111
            &   0775   ->    111111101
                ------------ 111111101     ->0775
                umask的作用就是抹去某些权限。


}
*/
//open函数调用头文件       
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
//close、read函数调用头文件
#include <unistd.h>

int main()
{   
    //打开一个文件
    int fd = open("a.txt", O_RDONLY);

    if(fd == -1){
        perror("open");
    }

    //读写操作

    //关闭
    close(fd);

    //创建一个新文件
    int fd_1 = open("create.txt", O_RDWR | O_CREAT, 0777);

    if(fd_1 == -1){
        perror("open");
    }

    //读写操作

    //关闭
    close(fd_1);

    return 0;
}
