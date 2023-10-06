/*
    #include <unistd.h>
    int access(const char *pathname, int mode);
        作用：判断调用进程对某个文件是否具有某个权限，或者判断文件是否存在。
        参数：
            -pathname：文件路径
            -mode：可以检查的权限
                R_OK：判断调用进程是否有读权限
                W_OK: 写权限
                X_OK: 执行权限
                F_OK：文件是否存在
        返回值：
            -成功： 0
            -失败： -1

    #include <sys/stat.h>
    int chmod(const char *pathname, mode_t mode);
        作用：修改文件权限
        参数：
            -pathname：文件路径】
            -mode：需要修改的权限值，八进制的数。
        返回值：0；-1。

    
    #include <unistd.h>
    #include <sys/types.h>
    int truncate(const char *path, off_t length);
        作用：缩减/拓展文件的尺寸到指定的大小
        参数：
            -path：文件路径
            -length：需要的最终的文件的大小，单位：字节
        返回值：0; -1.


*/

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    //判断文件是否存在
    int ret = access("cpy.txt", F_OK);
    if(ret == -1){
        perror("access");
        return -1;
    }
    printf("file exited!!\n");

    //修改文件权限
    int rett = chmod("cpy.txt", 0775);
    if(rett == -1){
        perror("chomod");
        return -1;
    }

    //缩减/拓展文件尺寸
    int rettt = truncate("a.txt", 5);
    if(rettt == -1){
        perror("truncate");
        return -1;
    }

    return 0;
}