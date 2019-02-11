#include <iostream>
#include"unp.h"
using namespace std;

class addr{

public :
    addrinfo(){} 
    ~addrinfo(){}
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int protocol;
    socklen_t ai_addrlen;
    char * ai_canonname;
    struct sockaddr *ai_addr;
    addr *ai_next;
};
int tcp_connect(const char* host,const char*serv){
    int sockfd,n;

}
int main(int argc,char ** argv)
{
    int sockfd;
    if(argc!=3){
        cout<<"The incorect use is tcp send <host><port>"<<endl;
    }
    sockfd = tcp_connect(argv[1],argv[2]);
    

}

