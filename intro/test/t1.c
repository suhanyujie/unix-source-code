/*
//编译方式
gcc -I../lib -g -O2 -D_REENTRANT -Wall -c -o t1.o t1.c
gcc t1.c -o t1.out
*/

#include "unp.h"

int main(int argc, char const *argv[])
{
    /* code */


    // readn();
    printf("hello world...\n");
    return 0;
}

//返回套接字的地址族
int sockfd_to_family(int sockfd)
{
    struct sockaddr_storage ss;
    socklen_t len;
    len = sizeof(ss);
    if(getsockname(sockfd, (SA *)&ss,&len)<0)
    {
        return -1;
    }
    return ss.ss_family;
}
