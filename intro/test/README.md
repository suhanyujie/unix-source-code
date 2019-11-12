### 编译参考
* 编译：`gcc intro/test/tcpClient.c -o tcpcli libunp.a -lresolv -lpthread`
* 编译：`gcc tcpClient.c -o tcpcli ../../libunp.a -lresolv -lpthread`
* 对于Server端的编译：`gcc tcpServer.c -o a.out ../../libunp.a -lresolv -lpthread`

### 查看本地套接字连接信息
* 查看本地套接字信息`netstat -anvp tcp | grep 9877`
* 查看端口情况:`lsof -i:9877`
* 使用 lsof 代替 netstat 进行查看：`lsof -nP -iTCP:端口号 -sTCP:LISTEN`

### 查看服务端进程
* 查看服务端进程 `pstree | grep "a.out"`
* 当关闭一个客户端时，查看计算机上的进程：`ps -axwwo user,pid,ppid,pgid,stat,command | grep 'a.out'`
* 此时可以看到`stat`项，之前的子进程的状态是`Z+`，也就是僵死状态
* 原因是，当我们关闭客户端时，服务端子进程会向父进程发送一个信号，但是我们再服务端的程序中，没有处理这个信号，导致子进程没有正常回收


## 其他信息
* 进程STAT列的"S"表示等待某些资源而睡眠





## 参开资料
* Mac的netstat和lsof使用 https://blog.csdn.net/JoshuaXin/article/details/84300934
