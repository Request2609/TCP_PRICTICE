#pragma once
#include<syslog.h>
#include "unp.h"
#include<memory>

class sockfd{

private :
    
    static shared_ptr<sockfd>sock ;
    
public :
    struct sockaddr_in serv ;
    int fd ; 
    sockfd() ; 
    ~sockfd() ; 
    static shared_ptr<sockfd> getsockfd(const char* ip, char* port ) ;
    //设置非阻塞
    void s_bind() ;
    void s_listen() ;
    
};

shared_ptr<sockfd> sockfd:: sock = NULL ;

sockfd :: sockfd()
{
    sock = NULL ;
}

shared_ptr<sockfd> sockfd :: getsockfd(const  char* ip, char* port )
{
    if(sock == NULL )
    {
        
        sock = make_shared<sockfd>() ;
        int flag = 1 ;
        sock->fd = socket( AF_INET, SOCK_STREAM, 0) ;
        int ret = setsockopt( sock->fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag) ) ; 
        if( ret < 0 )
        {
            cout << "build socketopt error!" << endl ;
            exit( 1 ) ;
        }
        cout << sock->fd <<endl ;
        
        if( sock->fd < 0 )
        {
            cout << "build socket error!" << endl ;
            exit( 1 ) ;
        }

        ( sock->serv ).sin_family = AF_INET ; 
        ( sock->serv ).sin_port = htons( atoi(port) ) ;
        ( sock->serv ).sin_addr.s_addr = inet_addr( ip );
        memset(sock->serv.sin_zero,'\0',8);
    }
    return sock ;
}

void sockfd :: s_bind()
{
    int ret = bind( sock->fd, ( struct sockaddr* )&serv, sizeof( serv ) ) ;
    if( ret < 0 )
    {   
        cout << "bind socket error!" << endl ;
        exit( 1 ) ;
    }
    
    cout << "binded......" << endl ;
}

void sockfd :: s_listen(){
    
    int ret = listen( sock->fd , 10) ;

    if( ret < 0 )
    {
        cout << "listen socket error!" << endl ;
        exit( 1 ) ;
    }
    
    cout << "listen......" << endl ;
}

sockfd :: ~sockfd()
{
    close( fd ) ;
    sock = NULL ;
}
