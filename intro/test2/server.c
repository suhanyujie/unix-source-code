#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_listenfd(void) 
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    int n = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&n,4);
    struct sockaddr_in sin;
    bzero(&sin,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(2019);
    sin.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd,(struct sockaddr *)&sin,sizeof(sin))<0) 
    {
        perror("bind error.");   
        exit(-1); 
    }
    if (listen(fd,100)) {
        perror("listen error.");
        exit(-1);
    }
    return fd;
}

void forward (int fd1,int fd2)
{
    //先侦查哪个fd是可读的，然后发给另一个
    char buffer[10240] = {0};
    fd_set fdset;
    while(1)
    {
        FD_ZERO(&fdset);
        FD_SET(fd1,&fdset);
        FD_SET(fd2,&fdset);
        int maxfd = (fd1>fd2 ? fd1 : fd2) + 1;
        int r = select(maxfd,&fdset,NULL,NULL,0);
        if (r<=0) break;
        //表示fd1可读
        if (FD_ISSET(fd1,&fdset))
        {
            int rlen = read(fd1,buffer,sizeof(buffer));
            if (rlen<=0) break;
            int wlen = write(fd2,buffer,rlen);
            if (wlen<=0) break;
        }
        //表示fd2可读
        if (FD_ISSET(fd2,&fdset))
        {
           int rlen = read(fd2,buffer,sizeof(buffer));
            if (rlen<=0) break;
            int wlen = write(fd1,buffer,rlen);
            if (wlen<=0) break;
        }
    }
}

void * handler(void *arg)
{
    int clientfd = (int)arg;
    char buffer[10240] = {0};
    int rlen = read(clientfd,buffer,sizeof(buffer));
    if (rlen<=0) return NULL;
    if (buffer[0]!= 5) return NULL;
    //将ack回复给客户端
    buffer[0] = 5;
    buffer[1] = 0;
    int wlen = write(clientfd,buffer,2);
    if (wlen != 2) return NULL;
    //接收客户端发来的连接请求
    rlen = read(clientfd, buffer,sizeof(buffer));
    if (rlen<=0) return NULL;
    if (buffer[0]!=5 || buffer[1]!=1) return NULL;
    //连接目标服务器
    struct sockaddr_in sin;
    bzero(&sin,sizeof(sin));
    sin.sin_family = AF_INET;
    //copy ip 
    memcpy(&sin.sin_addr,buffer+4,4);
    //拷贝端口
    memcpy(&sin.sin_port,buffer+8,2);
    int dstfd = socket(AF_INET,SOCK_STREAM,0);
    if (connect(dstfd,(struct sockaddr *)&sin,sizeof(sin))<0)
    {
        perror("dsrfd connect error.");
        return NULL;
    }
    printf("connect dstfd successful!\n");
    //回应连接请求
    bzero(buffer,sizeof(buffer));
    buffer[0] = 5;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 1;
    wlen = write(clientfd, buffer, 10);
    if (wlen != 10) return NULL;
    // 完成连接后，开始转发数据
    forward(dstfd,clientfd);
    close(dstfd);

    
    printf("received:%d\n", clientfd);
    // 关闭文件描述符，防止文件描述符被耗尽
    close(clientfd);
}

int main() {
    int listenfd = create_listenfd();
    while(1){
        int clientfd = accept(listenfd,NULL,NULL);
        //clientfd实际代表的是文件描述符
        printf("clentfd is :%d\n", clientfd);
        pthread_t tid;
        pthread_create(&tid,NULL,handler,(void *)clientfd);
    }

    return 0;
}

