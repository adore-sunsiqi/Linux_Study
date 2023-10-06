/*
#include <unistd.h>

extern char **environ;//环境变量，env 查看指令

int execl(const char *pathname, const char *arg, ..., (char *) NULL);
    -返回值
        -成功：没有返回值
        -失败：返回-1.
    -参数
        -pathname：可执行文件的路径
        -arg：第一个参数为可执行文件的名字，不带路径且以NULL结束。
        -file：如果参数包含/，则为路径名，否则就在PATH环境变量找
int execlp(const char *file, const char *arg, ..., (char *) NULL);
int execle(const char *pathname, const char *arg, ..., (char *) NULL, char *const envp[]);
int execv(const char *pathname, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
以上是标准C库函数.

int execve(const char *pathname, char *const argv[], char *const envp[]);
以上是Unix/Linux系统函数，

l：使用参数列表
p：使用文件名，从PATH进行寻找
v：构造一个指向各参数的指针数组，然后将这个数组的指针作为参数
e：多了envp[]数组，使用新的环境变量来代替调用进程的环境变量,PATH=/../
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    //创建一个子进程，在子进程执行exec函数族中的函数
    pid_t pid = fork();

    if(pid == -1){
        perror("fork");
        return -1;
    }else if(pid == 0){
        //子进程
        printf("child process, pid : %d\n", getpid());

        //execl("hello", "hello", NULL);
        //execl("/home/nana/linux/linux_duojincheng/hello", "hello", NULL);
        //execl("/bin/ls", "ls", "-l", NULL);
        //execl("/bin/ps", "ps", "aux", NULL);
        //perror("execl");

        //execlp("/bin/ls", "ls", "-l", NULL);
        //execlp("ls", "ls", "-l", NULL);//从环境变量里去找
        //execlp("/home/nana/linux/linux_duojincheng/hello", "hello", NULL);//可以执行，按路径
        //perror("execlp");

        //char * argv[] = {"/bin/ls", "-l", NULL};
        //execv("/bin/ls", argv);
        //char * argv[] = {"/home/nana/linux/linux_duojincheng/hello", "hello", NULL};
        //execv("/home/nana/linux/linux_duojincheng/hello", argv);//execv("hello", argv);
        //perror("execv");

        //char * argv[] = {"/bin/ls", "-l", NULL};
        //char * envp[] = {NULL, NULL};
        //execve("/bin/ls", argv, envp);
        //char * argv[] = {"hello", "hello", NULL};//char * argv[] = {"/home/nana/linux/linux_duojincheng/hello", "hello", NULL};
        //char * envp[] = {"PATH=/home/nana/linux/linux_duojincheng", NULL};
        //execve("hello", argv, envp);
        //perror("execve");


        int i ;
        for(i=0;i<4;i++){
            printf("sum = %d\n", ++i);
        }
    }else{
        //父进程
        printf("parent process, pid : %d\n", getpid());
        sleep(1);
    }


    return 0;
}