#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{   
    // 创建套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    // 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9999);

    // 绑定
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    
    // 通信
    while(1){
        // 接收数据
        char recvbuf[20];
        char ipbuf[16];
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);
        recvfrom(fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&clientaddr, &len);
        
        printf("client IP : %s, port : %d\n",
        inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
        ntohs(clientaddr.sin_port));

        printf("client say : %s\n", recvbuf);

        // 发送数据
        sendto(fd, recvbuf, strlen(recvbuf) + 1, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
    }

    close(fd);

    return 0;
}