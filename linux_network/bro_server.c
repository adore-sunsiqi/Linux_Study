// 广播

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

    // 设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    // 创建广播地址
    struct sockaddr_in broaddr;
    broaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.88.255", &broaddr.sin_addr.s_addr);
    broaddr.sin_port = htons(9999);


    
    // 通信
    int num = 0;
    while(1){
        // 发送数据
        char sendbuf[128];
        sprintf(sendbuf, " This is server : %d\n", num++);
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr *)&broaddr, sizeof(broaddr));
        printf("发送数据：%s\n", sendbuf);
        sleep(1);
    }

    close(fd);

    return 0;
}