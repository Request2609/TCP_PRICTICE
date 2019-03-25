#include <iostream>
#include"socketfd.h"

typedef void (*sighandler_t)(int) ;

sockfd* sock = NULL ;
void sig_handle( int signo )
{
    if( signo == SIGINT )
    {
        delete sock ;  
        exit(0) ;
    }
}

int main(int argc,char** argv)
{
    if( argc!=3 )
    {
        cout <<"NAME:" << argv[0] << "IP:"<<argv[1]<<"PORT:"<< argv[2] <<endl ;
        exit(1);
    }

    sock = sockfd::getsockfd(argv[1], argv[2]) ;

    sock->s_bind() ;
    sock->s_listen() ;
    
    int connfd ;
    
    if((connfd = accept(sock->fd, NULL, NULL)) < 0)
    {
        cout << connfd << "--------------" << endl ;
        cout << "accept error!" << endl ;
        exit(1) ;
    }
    char buf[20] = "hello client!" ;
    send(connfd, buf, 20, 0) ;
    //接收信号,创建子进程重启服务器
    while(1)
    {
         signal( SIGINT, sig_handle ) ;
    }
}
