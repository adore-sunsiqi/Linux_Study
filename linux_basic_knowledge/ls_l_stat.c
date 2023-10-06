// 用stat模拟实现ls -l指令
// -rw-rw-r-x 1 nana nana 1246  9月 10 10:33 cpy.txt

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc, char * argv[])
//argv[0] 指向程序运行的全路径名
//argv[1] 指向在DOS命令行中执行程序名后的第一个字符串
{
    //判断输入的参数是否正确
    if(argc < 2){
        printf("%s filename\n", argv[0]);
        return -1;
    }

    //通过stat函数获取用户传入的文件的信息
    struct stat st;
    int ret = stat(argv[1], &st);
    if(ret == -1){
        perror("stat");
        return -1;
    }

    //获取文件类型和文件权限
    char perms[11] = {0};//保存文件类型和文件权限的字符串
    //char类型需要多申请一位，最后一位用于存储 \0，用来表示是字符串
    switch (st.st_mode & S_IFMT)
    {
    case S_IFLNK:
        perms[0] = 'l';
        break;
    case S_IFDIR:
        perms[0] = 'd';
        break;
    case S_IFBLK:
        perms[0] = 'b';
        break;
    case S_IFREG:
        perms[0] = '-';
        break;
    case S_IFCHR:
        perms[0] = 'c';
        break;
    case S_IFIFO:
        perms[0] = 'p';
        break;
    case S_IFSOCK:
        perms[0] = 's';
        break;
    default:
        perms[0] = '?';
        break;
    }


    //判断文件的访问权限

    //文件所有者
    perms[1] = (st.st_mode & S_IRUSR ? 'r' : '-');
    perms[2] = (st.st_mode & S_IWUSR ? 'w' : '-');
    perms[3] = (st.st_mode & S_IXUSR ? 'x' : '-');

    //文件所在组
    perms[4] = (st.st_mode & S_IRGRP ? 'r' : '-');
    perms[5] = (st.st_mode & S_IWGRP ? 'w' : '-');
    perms[6] = (st.st_mode & S_IXGRP ? 'x' : '-');

    //其他人
    perms[7] = (st.st_mode & S_IROTH ? 'r' : '-');
    perms[8] = (st.st_mode & S_IWOTH ? 'w' : '-');
    perms[9] = (st.st_mode & S_IXOTH ? 'x' : '-');

    //获取硬链接数
    int linkNum = st.st_nlink;

    // 文件所有者
    char * fileUser = getpwuid(st.st_uid)->pw_name;

    // 文件所在组
    char * fileGrp = getgrgid(st.st_gid)->gr_name;

    //文件大小
    long int fileSize = st.st_size;

    //获取修改的时间
    char * time = ctime(&st.st_mtime);//ctime函数返回值带回车换行


    //解决gcc编译时回车换行问题
    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1);

    //输出
    char buf[1024];
    //格式化输出到字符串
    sprintf(buf, "%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);

    printf("%s\n", buf);

    return 0;
}
