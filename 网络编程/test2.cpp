#include"unp.h"

int main(int argc ,char **argv)
{
    
    if(argc <= 2){
        cout<<"usage : ./a.out <ip> <port>\n"<<endl;
        return 0 ;
    }

    const char *ip = argv[1] ;
    int port = atoi( argv[2] );
    
    struct sockaddr_in addr ;
    bzero(&addr , sizeof(addr)) ;
    addr.sin_family = AF_INET ;
    inet_pton(AF_INET , ip , &addr.sin_addr) ;
    addr.sin_port = htons(port) ;

    int sock = socket(PF_INET,SOCK_STREAM ,0 );

    assert(sock >= 0);

    int ret =  bind(sock,(struct sockaddr* )&addr , sizeof(addr)) ;
    assert(ret != -1);
    
    ret = listen(sock , 5) ;
    
    struct sockaddr_in cli ;
    socklen_t len = sizeof(cli) ;

    int connfd = accept( sock , (struct sockaddr*)&addr ,&len) ;
    
    if(connfd < 0 ){
        cout<<"errno is"<<errno<<endl;
        exit(1);
    }

    else{

        char remote[INET_ADDRSTRLEN] ;
    
        cout<<"IP:"<<inet_ntop(AF_INET,&cli.sin_addr,remote ,INET_ADDRSTRLEN)<<"\nPort:"<<ntohs(cli.sin_port)<<"\n";
        close(connfd) ;
    }

    close(sock) ;
    return 0;
}

