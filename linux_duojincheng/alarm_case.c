//1秒计算机能数多少个数

// 实际时间 = 内核态 + 用户态的运行时间

#include <unistd.h>
#include <stdio.h>

int main()
{
    alarm(1);
     
    int i = 0;
    while(1){
        printf("%d\n", i++);// 终端输出的时间
    }
    
    return 0;
}

// ./alarm_case >> a.txt 