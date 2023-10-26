/*
    #include <arpa/inet.h>
        
        // 转IP
       uint32_t htonl(uint32_t hostlong);
       uint32_t ntohl(uint32_t netlong);
       
        // 转端口port
       uint16_t htons(uint16_t hostshort);
       uint16_t ntohs(uint16_t netshort);

*/

#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    // 转端口
    unsigned short a = 0x0102;
    printf("a : %x\n", a);

    unsigned short b = htons(a);
    printf("htons : %x\n", b);

    unsigned short c = ntohs(a);
    printf("ntohs : %x\n", c);


    // 转IP
    unsigned char buf[4] = {192, 168, 1, 100};
    unsigned char * p = (char *)&buf;
    printf("%x.%x.%x.%x\n", *p, *(p+1), *(p+2), *(p+3));

    unsigned int sum = htonl(*(int *)buf);
    unsigned char * pp = (char *)&sum;
    printf("htonl : %d.%d.%d.%d\n", *pp, *(pp+1), *(pp+2), *(pp+3));

    sum = ntohl(*(int *)buf);
    pp = (char *)&sum;
    printf("ntohl : %d.%d.%d.%d\n", *pp, *(pp+1), *(pp+2), *(pp+3));

    return 0;
}





