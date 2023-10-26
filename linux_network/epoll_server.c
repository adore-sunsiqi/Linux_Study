#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>


int main()
{
    // 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY; //或者 = 0 // 0.0.0.0 任意一个地址
    saddr.sin_port = htons(9999);

    // 绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }
    
    // 调用epoll_create()创建一个epoll实例
    int epfd = epoll_create(200000);

    // 将监听的文件描述符的相关信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while(1){
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1){
            perror("epoll_wait");
            exit(-1);
        }
        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++){

            int curfd = epevs[i].data.fd;

            if( curfd == lfd){
                // 监听的文件描述符有数据到达，说明有客户端连接
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
                if(cfd == -1){
                    perror("accept");
                    exit(-1);
                }

           
                epev.events = EPOLLIN | EPOLLOUT;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }else{
                if(epevs[i].events & EPOLLOUT){
                    continue;
                }
                // 有数据到达，需要通信
                char buf[1024];
                int len = read(curfd, buf, sizeof(buf));
                if(len == -1){
                    perror("read");
                    exit(-1);
                } else if(len == 0){
                    // 断开连接
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                } else if(len > 0){
                    printf("read buf = %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }   
            }
        }
    }

    close(lfd);
    close(epfd);
    return 0;

}