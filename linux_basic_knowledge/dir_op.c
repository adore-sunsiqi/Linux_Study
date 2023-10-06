/*
    #include <sys/stat.h>
    #include <sys/types.h>
    int mkdir(const char *pathname, mode_t mode);
        作用：创建一个目录
        参数：
            -pathname：创建的目录的路径
            -mode：权限，八进制数
        返回值：0；-1；

    
    #include <unistd.h>
    int rmdir(const char *pathname);
        作用：删除空目录
        参数：
            —pathname：已存在的目录的路径。


    #include <stdio.h>
    int rename(const char *oldpath, const char *newpath);
        作用：目录重命名
        参数：
            -oldpath：旧目录名字
            -newpath：新目录名字


    #include <unistd.h>
    int chdir(const char *path);
        作用：修改进程的工作目录
        参数：
            -path：修改到的工作目录


    #include <unistd.h>
    char *getcwd(char *buf, size_t size);
        作用：获取当前的工作目录
        参数：
            -buf：保存的路径，指向的是一个数组（传出参数）
            -size：数组的大小
        返回值：返回的是指向的一块内存。

    
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *name);
        作用：打开一个目录
        参数：
            -name：需要打开的目录名称
        返回值：
            DIR * 类型，目录流
            失败返回NULL。

    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
        作用：读取目录中的数据，执行一次读取一次
        参数：
            -dirp：opendir返回值
        返回值：struct dirent，代表读取的文件的信息
            读取到末尾或者失败，则返回NULL


    #include <sys/types.h>
    #include <dirent.h>
    int closedir(DIR *dirp);
        作用：关闭目录
        参数：
            -dirp：opendir的返回值




*/


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int getFileNum(const char * path);

int main(int argc, char * argv[])
{
    if(argc < 2){
        printf("$s path\n", argv[1]);
        return -1;
    }

    //创建新目录
    int ret = mkdir("aaa", 0777);
    if(ret == -1){
        perror("mkdir");
        return -1;
    }
    printf("新目录创建成功！\n");

    //重命名目录
    int ret1 = rename("aaa", "bbb");
    if(ret1 == -1){
        perror("rename");
        return -1;
    }
    printf("目录重命名成功！\n");

    //删除空目录
    int ret11 = rmdir("bbb");
    if(ret11 == -1){
        perror("rmdir");
        return -1;
    }
    printf("空目录删除成功！\n");

    //获取当前的工作目录
    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("当前的工作目录是：%s\n", buf);

    //修改工作目录
    int rest = chdir("/home/nana/linux1");
    if(rest == -1){
        perror("chdir");
        return -1;
    }

    //创建一个新的文件
    int fd = open("chdir.txt", O_CREAT | O_RDWR, 0664);
    if(fd == -1){
        perror("open");
        return -1;
    }
    close(fd);

    //获取当前的工作目录
    char buf1[128];
    getcwd(buf1, sizeof(buf));
    printf("当前的工作目录是：%s\n", buf1);

    //读取某个目录下所有的普通文件的个数
    int num = getFileNum(argv[1]);
    printf("num = %d\n", num);
    
    return 0;
}

//用于获取目录下所有普通文件的个数
int getFileNum(const char * path){
   
    //1.打开目录
    DIR * dir = opendir(path);
    if(dir == NULL){
        perror("opendir");
        exit(0);
    }

    
    //记录普通文件的数量
    int total = 0;

    //2.读取
    struct dirent * rd;
    while((rd = readdir(dir)) != NULL){
        
        //3.获取名称，判断是./还是../。如果是，要忽略掉
        char * dname = rd -> d_name;
        if(strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0){
            continue;
        }

        //判断是否是普通文件，如果是目录递归调用getFileNum。
        if(rd -> d_type == DT_DIR)
        {
            //是目录，继续读取
            char newpath[256];
            sprintf(newpath, "%s/%s", path, dname);
            total += getFileNum(newpath);
            
        }

        if(rd -> d_type == DT_REG){
            //普通文件
            total++;
        }


    }
    closedir(dir);
    
    return total;
}