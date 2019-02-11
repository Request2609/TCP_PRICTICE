#include"unp.h"
#include<poll.h>

#define MAX_EVENT_NUMBER 256
#define _GUN_SOURCE 1  
//最大用户数量
#define USER_LIMIT 5 
//读缓冲区的大小
#define BUFFER_SIZE 64  
//文件秒符数量限制
#define FD_LIMIT 65535  

//客户数据,客户端socket地址,待写到客户端的数据的位置,从客户端读入的数据
struct client_data
{
    sockaddr_in addr ;
    char * write_buf ;
    char buf[ BUFFER_SIZE ] ;
};

//将套接字设置为非阻塞
int setnonblocking( int fd )
{
    int old_option = fcntl( fd , F_GETFL ) ;
    int new_option = old_option | O_NONBLOCK ;
    fcntl( fd , F_SETFL , new_option ) ;
    return old_option ;
}

//往epoll 树上加节点
void  addfd( int& epfd , int& fd )
{
    
    struct epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = EPOLLIN | EPOLLET ;
    epoll_ctl( epfd, EPOLL_CTL_ADD, fd, &ev ) ;
    setnonblocking( fd ) ;

}

int main( int argc, char** argv )
{
    
    if ( argc <= 2 )
    {
        cout<<"usage:name <ip><port>"<<endl ;
        return 1 ;
    }

    const char * ip = argv[1] ;
    int port = atoi( argv[2] ) ;
    int ret = 0 ;

    //设置相关信息
    //创建TCP套接字
    int lfd = socket( AF_INET, SOCK_STREAM, 0 ) ;
    struct sockaddr_in serv ;
    bzero( &serv, sizeof(serv) );
    serv.sin_family = AF_INET ;
    serv.sin_port = htons( port ) ;
    inet_pton( AF_INET, ip, &serv.sin_addr ) ;
    
    setnonblocking(lfd) ;   
    ret = bind( lfd, ( struct sockaddr* )& serv, sizeof(serv) ) ;
    assert( ret!=-1 ) ;
    ret = listen( lfd , 5 );
    assert( ret != -1 );
    
    //创建epoll 树处理事件
    epoll_event events[ MAX_EVENT_NUMBER ] ;
    int epfd = epoll_create(5) ;
    addfd(epfd , lfd );
    while( 1 )
    {
        int num = epoll_wait( epfd , events , MAX_EVENT_NUMBER , -1 )  ;

        assert( num >= 0 );
        for( int i = 0 ; i < num ; i++ )
        {
            int sockfd = events[i].data.fd ;

            if( sockfd == lfd )
            {
                struct sockaddr_in cli ;
                socklen_t len = sizeof( cli ) ;
                int connfd = accept( lfd , ( struct sockaddr* )&cli ,&len );
                addfd( epfd , connfd );
            }

            else if ( events[i].events & EPOLLIN )
            {

                char buf[BUFFER_SIZE] ;
               
                while(1){
                    memset( buf, '\0' ,BUFFER_SIZE);
                    ret = recv(sockfd ,buf , BUFFER_SIZE-1 , 0);
                    if(ret < 0){
                        
                        if((errno == EAGAIN) || (errno == EWOULDBLOCK ))
                        {
                            break ;
                        }
                        close(sockfd);
                        break ;
                    }
                    else if(ret == 0)
                    {
                        close(sockfd) ;
                    }
                    else
                    {
                        send( sockfd , buf , ret , 0 );
                    }
                }
            }
            else{
                cout<<"something else happen"<<endl ;
            }
        }
    }
    close(lfd) ;
    return 0;
}

