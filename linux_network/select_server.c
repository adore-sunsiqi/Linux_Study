#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
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
        exit(0);
    }

    // 3.监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(0);
    }
    
    // 创建一个fd_set的集合 存放的是需要检测的文件描述符
    fd_set rdset; // 用户修改rdset，内核不修改
    fd_set temp; // 让内核去修改临时集合
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while(1){

        temp = rdset;
        // 调用select系统函数，让内核检测哪些文件描述符有数据
        int ret = select(maxfd + 1, &temp, NULL, NULL, NULL);
        if(ret == -1){
            perror("select");
            exit(-1);
        } else if(ret == 0){
            // 超时事件到了 也没有检测到数据
            continue;
        } else if(ret > 0){
            // 检测到有文件描述符的对应的缓冲区数据发生了改变
            if(FD_ISSET(lfd, &temp)){
                // 表示有新的客户连接进来
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
                if(cfd == -1){
                    perror("accept");
                    exit(0);
                }
                // 将新的文件描述符加入到集合中
                FD_SET(cfd, &rdset);

                maxfd = maxfd > cfd ? maxfd : cfd;// 更新最大的文件描述符
            }
            for(int i = lfd + 1; i <= maxfd; i++){
                if(FD_ISSET(i, &temp)){
                    // 说明这个文件描述符i对应的客户端发来了数据
                    char buf[1024];
                    int len = read(i, buf, sizeof(buf));
                    if(len == -1){
                        
                        perror("read");
                        exit(-1);
                    } else if(len == 0){
                        // 断开连接
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i, &rdset);
                    } else if(len > 0){
                        printf("read buf = %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }
        }
    }
    close(lfd);
    
    return 0;

}