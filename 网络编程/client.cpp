#include <iostream>
#include<stdarg.h>
#include<errno.h>
#include"unp.h"
#define N 100
using namespace std;
class sockets{
public:
    int sockfd;
    struct sockaddr_in serv_addr;
    char recvline[N];
    sockets(){
        bzero(&serv_addr,sizeof(serv_addr));
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(sockfd==-1){
            perror("init_sock");
            exit(-1);
        }
    }
    ~sockets(){}
    int connectaddr(char ** argc);
    int getinfo(char buf[]);
};

int sockets::getinfo(char buf[]){
    
    int n;
    while((n=read(sockfd,buf,MAXLINE))>0){
        buf[n]=0;
        if(fputs(buf,stdout)==EOF){
            perror("fputs error!");
            return -1;
        }
    }
    if(n<0){
        perror("read error!");
        return -1;
    }
    cout<<buf<<endl;
    return 0;
}
int sockets::connectaddr(char**argc){

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);
    if(inet_pton(AF_INET,argc[1],&serv_addr)<0){    
        perror("inet_pton");
        return -1;
    } 
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(sockaddr_in))<0) {
        perror("connect");
        return -1;
        } 
    return 0;
}
int main(int argv,char ** argc)
{
    if(argv!=2){
        perror("usage:run-filename IP");
        exit(0);
    }
    sockets ss;
    if(ss.connectaddr(argc)<0){
        exit(-1);
    }
    char buf[100];
    if(ss.getinfo(buf)<0) {
        exit(-1);
    }   
    return 0;
}

