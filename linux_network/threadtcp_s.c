// 多线程实现并发服务器
// 主线程负责监听，子线程负责通信

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

// 子线程需要的信息
struct sockInfo{
    int fd;// 通信的文件描述符
    pthread_t tid;//线程号
    struct sockaddr_in addr;//IP地址
};

struct sockInfo sockInfos[2];// 同时支持2个客户端进行连接

void * working(void * arg){
    // 子线程和客户端通信  需要cfd,clientaddr,tid这些信息
    
    // 获取客户端信息
    struct sockInfo * pinfo = (struct sockInfo *)arg;

    char clientip[16];
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, clientip, sizeof(clientip));
    unsigned short clientport = ntohs(pinfo->addr.sin_port);
    printf("client ip is %s, client port is %d\n", clientip, clientport);

    char recvbuf[1024];

    while(1){

        // 获取客户端的数据
        int len_r = read(pinfo->fd, recvbuf, sizeof(recvbuf));
        if(len_r == -1){
            perror("read");
            exit(-1);
        } else if(len_r > 0){
            printf("recv client data : %s\n", recvbuf);
        } else if(len_r == 0){
            // 表示客户端断开连接
            printf("客户端断开连接\n");
            break;
        }

        // 给客户端发送数据
        write(pinfo->fd, recvbuf, strlen(recvbuf) + 1);
    }
    close(pinfo->fd);
    pinfo->fd = -1;       
    return NULL;
}


int main()
{
    // 1.创建用于监听的套接字socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
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
        exit(-1);
    }

    // 3.监听
    ret = listen(lfd, 128);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    // 初始化数据
    int max = sizeof(sockInfos) / sizeof(sockInfos[0]);
    for(int i = 0; i < max; i++){
        bzero(&sockInfos[i], sizeof(sockInfos[i]));
        sockInfos[i].fd = -1; // 表示可用，描述符无效，
        sockInfos[i].tid = -1;
    }

    // 4.不断循环等待接收客户端连接
    while(1){
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);

        struct sockInfo * pinfo;
        for(int i = 0; i < max; i++){
            if(sockInfos[i].fd == -1){
                pinfo = &sockInfos[i];
                break;
            }
            if(i == max -1){
                sleep(1);
                i = -1;
            }
        }

        pinfo->fd = cfd;

        memcpy(&pinfo->addr, &clientaddr, len);

        pthread_create(&pinfo->tid, NULL, working, pinfo);

        pthread_detach(pinfo->tid);
        
    }     
    close(lfd);
    return 0;
}