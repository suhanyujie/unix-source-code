#include "unp.h"

int main(int argc, char const *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    printf("this argc is:%d\n", argc);
    if (argc != 2)
    {
        err_quit("usage:tcpcli <IPaddress>");
    }
    sockfd = Socket(AF_INET,SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    exit(0);

    return 0;
}
