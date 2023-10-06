//匿名映射：不需要通过文件描述符进行操作

//匿名映射只能进行父子进程间的通信

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
    int len = 4096;
    void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    //父子进程间通信

    pid_t pid = fork();
    if(pid > 0){
        strcpy((char *)ptr, "hello, world");
        wait(NULL);
    }else if(pid == 0){
        sleep(1);
        printf("%s\n", (char *)ptr);
    }else{
        perror("fork");
        exit(0);
    }

    int ret = munmap(ptr, len);
    if(ret == -1){
        perror("munmap");
        exit(0);
    }


    return 0;
}