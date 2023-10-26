// 实现本地进程间通信

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main()
{
    unlink("server.sock");
    // 创建监听的套接字
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    // 绑定本地套接字文件
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 监听
    ret = listen(lfd, 100);
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 等待客户端连接
    struct sockaddr_un clientaddr;
    int len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);

    if(cfd == -1){
        perror("accept");
        exit(-1);
    }
    printf("client socket filename : %s\n", clientaddr.sun_path);


    // 通信
    while(1){
        char buf[128];
        int len = recv(cfd, buf, sizeof(buf), 0);// 会读取\0
        if(len == -1){
            perror("recv");
            exit(-1);
        }else if(len == 0){
            printf("client closed...\n");
            break;
        }else if(len > 0){
            printf("client say : %s\n", buf);
            send(cfd, buf, len, 0);
        }
    }

    close(cfd);
    close(lfd);

    return 0;
}