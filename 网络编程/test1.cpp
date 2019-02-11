#include <iostream>
#include<assert.h>

#include "unp.h"

static bool stop = false ;

static void handle_term(int sig){

    stop = true ;
}

int main(int argc,char ** argv)
{
    if( argc < 2 ){
        cout<<"usage:./a.out <ip> <port>"<<endl;
        return 0 ;
    }
    
    int sock = socket(AF_INET , SOCK_STREAM , 0);
    
    assert(sock!=-1);

    const char *ip = argv[1];
    
    int port = atoi( argv[2] ) ;
    struct sockaddr_in addr ;
    bzero(&addr , sizeof(addr));

    addr.sin_family = AF_INET ;
    
    inet_pton(AF_INET, ip , &addr.sin_addr) ;

    addr.sin_port = htons(port);

    int ret = bind(sock , (struct sockaddr *)&addr ,sizeof(addr));
    assert(ret!=-1);

    ret = listen(sock, 9);
    assert(ret!=-1);
    
    while(!stop){
        sleep((1));
    }

    close(sock);
    return 0;
}

