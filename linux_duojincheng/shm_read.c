/*
#include <sys/ipc.h>
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
    - 作用：创建一个新的共享内存段，或者获取一个既有的共享内存段的标识
            新创建的共享内存段的数据都会被初始化为0
    - 参数：
        - key：key_t是一个整型，通过这个找到或创建一个共享内存。
                一般使用16进制表示，非0值
        - size：共享内存的大小。单位是分页的大小。4k的整数倍
        - shmflg：共享内存的属性
            - 访问权限：
            - 附加属性：创建/判断共享内存是否存在
                - 创建：IPC_CREAT
                - 判断：IPC_EXCL，需要和IPC_CREAT一起使用
    - 返回值：
        成功：> 0 . 共享内存的标识
        失败：-1

*/
/*
#include <sys/types.h>
#include <sys/shm.h>

void *shmat(int shmid, const void *shmaddr, int shmflg);
    - 作用：跟当前进程进行关联
    - 参数：
        - shmid：共享内存的标识id，由shmget返回值获取。
        - shmaddr：申请的共享内存的起始地址，由内核去指定，NULL
        - shmflg：对共享内存的操作。必须要有SHM_RDONLY。设置为0.
    - 返回值：
        成功：返回共享内存的起始地址。
        失败：-1（void*）

int shmdt(const void *shmaddr);
    - 作用：解除当前进程和共享内存的关联
    - 参数：
        - shmaddr：共享内存的首地址
    - 返回值：
        成功：0
        失败：-1

*/
/*
#include <sys/ipc.h>
#include <sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    - 作用：操作共享内存。共享内存要删除才能消失，创建共享内存的进程是否被销毁对共享内存没有影响
    - 参数：
        - shmid：共享内存的id
        - cmd：要做的操作；
            - IPC_STAT：获取共享内存的状态
            - IPC_SET：设置共享内存的状态
            - IPC_RMID：标记共享内存需要被销毁
        - buf：需要设置或者获取的共享内存的属性信息
            - IPC_STAT：buf存储信息
            - IPC_SET：buf中需要初始化数据，设置到内核
            - IPC_RMID：没有用，buf设置为NULL
*/
/*
#include <sys/types.h>
#include <sys/ipc.h>

key_t ftok(const char *pathname, int proj_id);

*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    //1.获取一个共享内存
    int shmid = shmget(100, 0, IPC_CREAT);
    if(shmid == -1){
        perror("shmget");
        exit(0);
    }
    printf("shmid : %d\n", shmid);
    
    //2.和当前进程关联
    void * ptr = shmat(shmid, NULL, 0);
    if(ptr == (void *)-1){
        perror("shmat");
        exit(0);
    }

    //3.读数据
    printf("read data : %s\n", (char *)ptr);
    

    printf("按任意键继续\n");
    getchar();

    //4. 解除关联
    shmdt(ptr);

    
    //5.删除共享内存
    int ret = shmctl(shmid, IPC_RMID, NULL);
    if(ret == -1){
        perror("shmctl");
        exit(0);
    }
   
    return 0;
}
