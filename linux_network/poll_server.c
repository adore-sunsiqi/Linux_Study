#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
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

    // 3.监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }
    
    // 初始化检测的文件描述符
    struct pollfd fds[1024]; // 1024个客户端
    for(int i = 0; i < 1024; i++){
        fds[i].fd = -1; // 表示可用
        fds[i].events = POLLIN;
    }

    fds[0].fd = lfd; // 把监听的文件描述符加入

    int nfds = 0;

    while(1){

        
        // 调用poll系统函数，让内核检测哪些文件描述符有数据
        int ret = poll(fds, nfds + 1, -1);
        if(ret == -1){
            perror("poll");
            exit(-1);
        } else if(ret == 0){
            // 超时事件到了 也没有检测到数据
            continue;
        } else if(ret > 0){
            // 检测到有文件描述符的对应的缓冲区数据发生了改变
            if(fds[0].revents & POLLIN){// 不能用==，因为可能包含pollout等其他事件
                // 表示有新的客户连接进来
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
                if(cfd == -1){
                    perror("accept");
                    exit(-1);
                }
                // 将新的文件描述符加入到集合中
                for(int i = 1; i < 1024; i++){
                    if(fds[i].fd == -1){
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        nfds = nfds > i ? nfds : i; 
                        break;
                    }
                }

                
                // nfds = nfds > cfd ? nfds : cfd;
            }
            for(int i = 1; i <= nfds; i++){
                if(fds[i].revents & POLLIN){
                    // 说明这个文件描述符i对应的客户端发来了数据
                    char buf[1024];
                    int len = read(fds[i].fd, buf, sizeof(buf));
                    if(len == -1){
                        
                        perror("read");
                        exit(-1);
                    } else if(len == 0){
                        // 断开连接
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    } else if(len > 0){
                        printf("read buf = %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf) + 1);
                    }
                }
            }
        }
    }
    close(lfd);
    
    return 0;

}