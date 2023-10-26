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

    // 绑定本地的IP和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9999);
    
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 加入到多播组
    struct ip_mreq op;
    inet_pton(AF_INET, "239.0.0.10", &op.imr_multiaddr.s_addr);
    op.imr_interface.s_addr = INADDR_ANY;
    
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &op, sizeof(op));

    // 通信
    while(1){

        char recvbuf[128];
        // 接收数据
        recvfrom(fd, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        printf("server say : %s\n", recvbuf);
        sleep(1);
    }

    close(fd);

    return 0;
}