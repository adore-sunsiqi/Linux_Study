// 判断数据是大端字节序还是小端字节序

#include <stdio.h>

int main()
{

    union {
        short value; // 2字节
        char bytes[sizeof(short)]; // char[2]
    }test; // 直接定义变量，只能用一次
    
    test.value = 0x0102;
    if((test.bytes[0] == 1) && (test.bytes[1] == 2))
    {
        printf("大端字节序\n");
    }else if((test.bytes[0] == 2) && (test.bytes[1] == 1)){
        printf("小端字节序\n");
    }else{
        printf("未知\n");
    }
  
    
    
    return 0;

}