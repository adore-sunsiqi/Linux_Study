// LT 工作模式

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>


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
    int epfd = epoll_create(200);

    // 将监听的文件描述符的相关信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while(1){
        int ret = epoll_wait(epfd, epevs, 1024, -1); // 检测
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

                // 设置cfd非阻塞
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);
           
                epev.events = EPOLLIN | EPOLLET;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }else{
                if(epev.events & EPOLLOUT){
                    continue;
                }
                // 有数据到达，需要通信，循环读取数据
                char buf[5];
                int len = 0;
                while((len = read(curfd, buf, sizeof(buf))) > 0){
                    // 打印数据
                    
                    // printf("read buf = %s\n", buf); // 还有数据也不会提醒
                    write(STDOUT_FILENO, buf, len);
                    write(curfd, buf, len);
                }
                if(len == 0){
                    printf("client closed...\n");
                }else if(len == -1){
                    if(errno == EAGAIN){
                        printf("data over...\n");
                    }else{
                        perror("read");
                        exit(-1);
                    }   
                }
                      
            }
        }
    }

    close(lfd);
    close(epfd);
    return 0;

}