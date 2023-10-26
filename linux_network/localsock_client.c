// 实现本地进程间通信

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main()
{
    unlink("client.sock");
    // 创建套接字
    int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(cfd == -1){
        perror("socket");
        exit(-1);
    }

    // 绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_INET;
    strcpy(addr.sun_path, "client.sock");
    int ret = bind(cfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 连接服务器
    struct sockaddr_un seraddr;
    seraddr.sun_family = AF_LOCAL;
    strcpy(seraddr.sun_path, "server.sock");
    ret = connect(cfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    // 通信
    int num = 0;
    while(1){
        char buf[128];
        sprintf(buf, "i am client %d\n", num++);
        send(cfd, buf, strlen(buf) + 1, 0);
        printf("client say : %s\n", buf);

        int len = recv(cfd, buf, sizeof(buf), 0);// 会读取\0
        if(len == -1){
            perror("recv");
            exit(-1);
        }else if(len == 0){
            printf("server closed...\n");
            break;
        }else if(len > 0){
            printf("server say : %s\n", buf);
        }
        sleep(1);
        
    }


    close(cfd);


    return 0;
}