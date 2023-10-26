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

    
    char recvbuf[10];
    // 程序里执行一次write，那么每执行一次程序，覆盖写入
    // 程序里执行多次write,那么就附加写入
    
    while(1){
        // 3.给服务器发送数据
        char * data = "hello, i am client";
    
        write(fd, data, strlen(data) + 1);
        sleep(1);
        // 4.获取服务器端的数据
        // char recvbuf[12] = {0};
        int len_r = read(fd, recvbuf, sizeof(recvbuf));
        if(len_r == -1){
            perror("read");
            exit(0);
        } else if(len_r > 0){
            printf("recv server data : %s\n", recvbuf);
        } else if(len_r == 0){
            // 表示服务器端断开连接
            printf("服务器断开连接\n");
        }

    }
   

    
    
    // 7.关闭连接
    close(fd);
   
    
    return 0;
}

