#include "unp.h"
// #include "../../unixdomain/strecho.c"

int main(int argc, char const *argv[])
{
    int listenfd,connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;

    listenfd = Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    Bind(listenfd,(SA *)&servaddr,sizeof(servaddr));
    Listen(listenfd,LISTENQ);
    printf("serv will start at port:%d\n", SERV_PORT);
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *)&cliaddr,&clilen);
        if ((childpid = Fork()) == 0) {
            Close(listenfd);
            str_echo(connfd);
            //printf("connfd is :%d\n", connfd);
            exit(0);
        }
        Close(connfd);
    }

    return 0;
}
