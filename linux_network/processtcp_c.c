// 实现TCP通信的客户端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    // 1.创建套接字socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    // 2.连接到服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.88.130", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1){
        perror("connect");
        exit(0);
    }

    
    char recvbuf[1024];// 读、写数据会自动覆盖缓冲区
    int i = 0;
    while(1){
        // 3.给服务器发送数据
        sprintf(recvbuf, "send data: %d\n", i++);
        write(fd, recvbuf, strlen(recvbuf) + 1); // 将字符串和 \0 一起写入recvbuf
        // write函数不会检查内存泄露
        
        // 4.获取服务器端的数据
        int len_r = read(fd, recvbuf, sizeof(recvbuf));
        if(len_r == -1){
            perror("read");
            exit(0);
        } else if(len_r > 0){
            printf("recv server data : %s\n", recvbuf);
        } else if(len_r == 0){
            // 表示服务器端断开连接
            printf("服务器断开连接\n");
            break;
        }
        usleep(1000);
    }
   

    
    
    // 7.关闭连接
    close(fd);
   
    
    return 0;
}
