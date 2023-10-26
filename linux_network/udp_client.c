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

    // 服务器地址信息
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.88.130", &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999);
    

    int i = 0;
    // 通信
    while(1){

        char sendbuf[128];
        sprintf(sendbuf, "i am client : %d \n", i++);
        // 发送数据
        sendto(fd, sendbuf, strlen(sendbuf) + 1, 0, (struct sockaddr *)&saddr, sizeof(saddr));
        
        // 接收数据
        recvfrom(fd, sendbuf, sizeof(sendbuf), 0, NULL, NULL);
        printf("server say : %s\n", sendbuf);
        sleep(1);
    }

    close(fd);

    return 0;
}