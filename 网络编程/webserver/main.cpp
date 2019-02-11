#include"unp.h"
#include"ThreadPool.h"
#include"func.h"

int main( int argc, char **argv )
{   
    chdir("serverHttp");
    

    if( argc != 3 )
    {
        cout << "usage:<FILE><IP><PORT>" << endl;
        return 0 ;
    }

    char* ip = argv[1] ;
    int port = atoi( argv[2] );
     
    int sfd = sock_init( ip, port ) ;
    
    epoll_event ev_fds[MAX]  ; 
    int epfd = epoll_create( MAXLINE ) ;
   
    add_fd(epfd , sfd);
    
    run_epoll( epfd, ev_fds , sfd ) ;
     
    close(sfd) ;
}

