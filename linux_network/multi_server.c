// 多播

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

    // 设置组播属性
    struct in_addr imr_multiaddr;
    inet_pton(AF_INET, "239.0.0.10", &imr_multiaddr.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    // 初始化客户端的地址信息
    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "239.0.0.10", &clientaddr.sin_addr.s_addr);
    clientaddr.sin_port = htons(9999);


    
    // 通信
    int num = 0;
    while(1){
        // 发送数据
        char sendbuf[128];
        sprintf(sendbuf, " This is server : %d\n", num++);
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        printf("多播的数据：%s\n", sendbuf);
        sleep(1);
    }

    close(fd);

    return 0;
}