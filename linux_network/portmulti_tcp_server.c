/*
问题：在计算机网络中，每个网络连接都需要使用唯一的端口号来标识应用程序或服务。
    当多个应用程序或服务需要监听相同的端口号时，传统上会导致冲突，因为一个端口只能被一个应用程序或服务占用。
    冲突比如：端口未成功释放

解决：端口复用技术允许多个应用程序或服务共享同一个端口号，而不会发生冲突。
    它通过区分连接的目标IP地址或协议来将传入的连接路由到正确的应用程序或服务。

实现方式：
    1、多路复用（Multiplexing）：多路复用是一种将多个连接复用到同一个端口的技术。
    它使用传输层协议（如TCP或UDP）的多路复用特性，通过在每个连接上使用不同的标识符（如源IP地址和端口号）来区分连接。
    服务器应用程序可以使用多路复用技术来同时监听和处理多个连接，从而实现端口复用。

    2、端口转发（Port Forwarding）：端口转发是一种将传入连接转发到不同目标的技术。
    它使用网络地址转换（NAT）或代理服务器来接收传入连接，并根据预先定义的规则将连接转发到不同的目标地址和端口。
    通过端口转发，可以将传入的连接路由到正确的应用程序或服务，实现端口复用。
*/

#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);

    if(lfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    
    //int optval = 1;
    //setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        return -1;
    }

    // 监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        return -1;
    }

    // 接收客户端连接
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
    if(cfd == -1) {
        perror("accpet");
        return -1;
    }

    // 获取客户端信息
    char cliIp[16];
    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cliIp, sizeof(cliIp));
    unsigned short cliPort = ntohs(cliaddr.sin_port);

    // 输出客户端的信息
    printf("client's ip is %s, and port is %d\n", cliIp, cliPort );

    // 接收客户端发来的数据
    char recvBuf[1024] = {0};
    while(1) {
        int len = recv(cfd, recvBuf, sizeof(recvBuf), 0);
        if(len == -1) {
            perror("recv");
            return -1;
        } else if(len == 0) {
            printf("客户端已经断开连接...\n");
            break;
        } else if(len > 0) {
            printf("read buf = %s\n", recvBuf);
        }

        // 小写转大写
        for(int i = 0; i < len; ++i) {
            recvBuf[i] = toupper(recvBuf[i]);
        }

        printf("after buf = %s\n", recvBuf);

        // 大写字符串发给客户端
        ret = send(cfd, recvBuf, strlen(recvBuf) + 1, 0);
        if(ret == -1) {
            perror("send");
            return -1;
        }
    }
    
    close(cfd);
    close(lfd);

    return 0;
}

