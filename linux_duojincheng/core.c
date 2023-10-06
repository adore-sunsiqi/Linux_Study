//无法生成core文件的问题！
//cat /proc/sys/kernel/core_pattern  
//输出 !/usr/share/apport %p %s %c %d %P %E
//原因是Ubuntu预装了apport错误收集系统
//解决 sudo service apport stop(start)


#include <stdio.h>
#include <string.h>

int main()
{
    char * buf;
    strcpy(buf, "hello!!!");
    return 0;
}