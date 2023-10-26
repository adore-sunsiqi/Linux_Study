// TCP通信服务器端的多进程实现
// 父进程进行监听；多个子进程进行通信：接收到一个客户端连接就创建一个子进程

// 实现TCP通信多进程的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void recylechild(int arg){
    while(1){
        int ret = waitpid(-1, NULL, WNOHANG);
        if(ret == -1){// 所有子进程都被回收
            break;
        } else if(ret == 0){
            break;// 还有子进程活着
        }else if(ret > 0){
            printf("child process %d 被回收\n", getpid());
        }
    }
}
int main()
{
    // 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recylechild;
    sigaction(SIGCHLD, &act, NULL);

    // 1.创建用于监听的套接字socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "192.168.88.130", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY; //或者 = 0 // 0.0.0.0 任意一个地址
    saddr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(0);
    }

    // 3.监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(0);
    }

    // 4.不断循环等待接收客户端连接
    while(1){
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int cfd = accept(lfd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
        // 该系统调用被信号打断之后，即产生软中断，accept将不阻塞，会产生错误
        if(cfd == -1){
            if(errno == EINTR){ // 处理SIGCHLD信号的影响
                continue;
            }
            perror("accept");
            exit(0);
        } 
        
        pid_t pid = fork();
        if(pid == 0){
            // 子进程
            // 输出客户端信息
            char clientip[16];
            inet_ntop(AF_INET, &clientaddr, clientip, sizeof(clientip));
            unsigned short clientport = ntohs(clientaddr.sin_port);
            printf("client ip is %s, client port is %d\n", clientip, clientport);

            char recvbuf[1024] = {0};

            while(1){

                // 5.获取客户端的数据
                int len_r = read(cfd, recvbuf, sizeof(recvbuf));
                // recv();
                if(len_r == -1){
                    perror("read");
                    exit(0);
                } else if(len_r > 0){
                    printf("recv client data : %s\n", recvbuf);
                } else if(len_r == 0){
                    // 表示客户端断开连接
                    printf("客户端断开连接\n");
                    break;
                }

                // 6.给客户端发送数据
                write(cfd, recvbuf, strlen(recvbuf) + 1);
            }
            close(cfd);
            exit(0);
        }     
    }
    
    close(lfd);  
    return 0;
}