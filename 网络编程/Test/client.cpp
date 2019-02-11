#include "unp.h"
#include<sys/poll.h>
#define BUFFER_SIZE 64


int main( int argc, char **argv )
{

    if ( argc != 3 ){
            
        cout << "Usage:<name><ip><port>" << endl;
        return 0 ;
    }

    const char * ip = argv[1] ;
    int port = atoi( argv[2] ) ;
    
    struct sockaddr_in cli ;
    bzero( &cli, sizeof(cli) );
    cli.sin_family = AF_INET ;
    cli.sin_port = htons( port ) ;
    inet_pton( AF_INET, ip, &cli.sin_addr ) ;
    
    int sockfd = socket( AF_INET, SOCK_STREAM , 0 ) ;
    
    assert( sockfd != -1 ) ;

    if ( connect( sockfd , (struct sockaddr* )&cli ,sizeof(cli)) < 0 ){
        cout << "connect failed "<< endl ;
        close( sockfd ) ;
        exit( 1 ) ;
    }

    //注册文件描述符0和文件描述符sockfd上的可读事件
    pollfd fds[2] ;
    fds[0].fd = 0 ;
    fds[0].events = POLLIN ;
    fds[0].revents = 0 ;
    fds[1].fd = sockfd ;
    fds[1].events = POLLIN | POLLRDHUP ;
    fds[1].revents = 0 ;
    char read_buf[BUFFER_SIZE] ;
    int pipefd[2] ;
    int ret = pipe( pipefd ) ;
    
    while ( 1 )
    {
        ret = poll( fds, 2, -1 );
        if( ret < 0 )
        {
            cout << "Poll failure\n" << endl ;
            break ;
        }
        
        if ( fds[1].revents & POLLRDHUP ){

            cout<<"server close connection!"<<endl ;
            break ;
        }

        if ( fds[1].revents & POLLRDHUP )
        {
            cout<<" server close the connection!"<< endl ;
            break ;
        }

        else if ( fds[1].revents & POLLIN )
        {
            memset( read_buf, '\0', BUFFER_SIZE );
            recv ( fds[1].fd, read_buf, BUFFER_SIZE-1, 0 ) ;
            cout<<read_buf<<endl ;
        }

        if ( fds[0].revents & POLLIN )
        {
            //使用splice 使用户输入的数据直接写道内核缓冲区
            ret = splice( 0, NULL, pipefd[1], NULL, 32768 ,SPLICE_F_MORE | SPLICE_F_MOVE);
            ret = splice( pipefd[0], NULL, sockfd, NULL, 32768 ,SPLICE_F_MORE | SPLICE_F_MOVE);
        }
    }

        close( sockfd ) ;
        return 0 ;
}

