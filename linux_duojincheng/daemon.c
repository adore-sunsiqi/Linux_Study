// 写一个守护进程，每隔2秒获取一下系统时间，并写入磁盘文件

// 为什么连续两次执行可执行程序后，该进程会结束？

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void work(int num){
    //捕捉到信号后，获取系统时间，写入磁盘文件
    time_t tm = time(NULL);
    struct tm * loc = localtime(&tm);//格式转换
    /*
    char buf[1024];
    sprintf(buf, "%d-%d-%d %d:%d:%d\n", loc->tm_year, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
    printf("%s\n", buf);
    */
   char * str = asctime(loc);// 将tm类型的数据格式化为字符串
   int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
   write(fd, str, strlen(str));
   close(fd);
}

int main()
{
    // 1.创建子进程，杀死父进程
    pid_t pid = fork();

    if(pid > 0){
        exit(0);
    }

    // 2.将子进程重新创建一个会话。保证创建会话的进程不是进程组的首进程
    setsid();//新的会话会脱离原来的控制终端

    // 3、设置掩码
    umask(022);

    // 4. 更改工作目录
    chdir("/home/nana/");

    // 5. 关闭、重定向文件描述符
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);


    // 6. 核心业务逻辑

    // 捕捉定时器信号
    struct sigaction act;
    act.sa_flags  = 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);

    // 创建定时器
    struct itimerval val;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &val, NULL);

    while(1){
        sleep(10);
    }

    return 0;
}