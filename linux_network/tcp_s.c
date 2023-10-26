// 实现TCP通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    // 1.创建用于监听的套接字socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(0);
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

    // 4.接收客户端连接  阻塞模型 效率低下
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
    if(cfd == -1){
        perror("accept");
        exit(0);
    }

    // 输出客户端信息
    char clientip[16];
    inet_ntop(AF_INET, &clientaddr, clientip, sizeof(clientip));
    unsigned short clientport = ntohs(clientaddr.sin_port);
    printf("client ip is %s, client port is %d\n", clientip, clientport);
    
    
    char recvbuf[10];
    
    while(1){
        // 5.获取客户端的数据
        // char recvbuf[12] = {0};
        int len_r = read(cfd, recvbuf, sizeof(recvbuf));
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
        char * data = "hello, i am server";
        write(cfd, data, strlen(data) + 1);
       
    }
    

    

    // 7.关闭连接
    close(cfd);
    close(lfd);
    
    return 0;
}