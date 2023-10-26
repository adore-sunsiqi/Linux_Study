#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>
#include "locker.h"
#include "threadpool.h"
#include "http_con.h"

#define MAX_FD 65535 // 最大文件描述符的个数
#define MAX_EVENT_NUMBER 10000 // 监听的事件的最大数量

// 添加信号捕捉
void addsig(int sig, void(handler)(int)){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

// 添加文件描述符到epoll中
extern void addfd(int epollfd, int fd, bool one_shot);
// 从epoll中删除文件描述符
extern void removefd(int epollfd, int fd);
// 从epoll中修改文件描述符
extern void modfd(int epollfd, int fd, int ev);


int main(int argc, char* argv[]){

    if(argc <= 1){
        // 至少要上传一个端口号，否则只上传一个 程序执行命令
        printf("按照如下格式运行: %s port_number\n", basename(argv[0])); // argv[0]存的是程序名
        exit(-1);
    }

    // 获取端口号
    int port = atoi(argv[1]);

    // 对sigpipe信号进行处理
    addsig(SIGPIPE, SIG_IGN);

    // 创建线程池，初始化
    threadpool<http_con> * pool = NULL;

    try{
        pool = new threadpool<http_con>;
    }catch(...){
        return 1;
    }


    // 创建一个数组，保存客户端信息
    http_con * users = new http_con[MAX_FD];

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    // 设置端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 监听
    listen(listenfd, 5);

    // 创建epoll对象，事件数组，将监听的文件描述符添加到epoll对象中
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);

    // 将监听的文件描述符添加到epoll对象中
    addfd(epollfd, listenfd, false);

    http_con::m_epollfd = epollfd;

    while(true){
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if((num < 0) && (errno != EINTR)){
            printf("epoll failure\n");
            break;
        }

        // 循环遍历事件数组
        for(int i = 0; i < num; i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                // 有客户端连接进来
                struct sockaddr_in client_address;
                socklen_t clientaddrlen = sizeof(client_address);
                int confd = accept(listenfd, (struct sockaddr *)&client_address, &clientaddrlen);
            
                if(http_con::m_user_count >= MAX_FD){
                    // 目前连接数满了
                    // 给客户端回写一个信息：服务器内部正忙。
                    close(confd);
                    continue;
                }

                // 将新的客户的数据初始化，放到数组当中
                users[confd].init(confd, client_address);
            } else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                // 对方异常断开或者错误等事件
                users[sockfd].close_con();
            } else if(events[i].events & EPOLLIN){
                if(users[sockfd].read()){
                    // 一次性把所有数据都读完
                    pool->append(users + sockfd);
                }else{
                    users[sockfd].close_con();
                }
            } else if(events[i].events & EPOLLOUT){
                // 一次性把所有数据都写完
                if(!users[sockfd].write()){
                    users[sockfd].close_con();
                }
            }
        }

    }

    close(epollfd);
    close(listenfd);
    delete[] users;
    delete pool;

    return 0;
}