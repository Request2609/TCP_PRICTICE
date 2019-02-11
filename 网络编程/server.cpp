#include <iostream>
#include"unp.h"
using namespace std;
#define N 100
class sockets{
public:
    int sockfd,connfd;
    socklen_t len;
    time_t times;
    struct sockaddr_in servaddr,cliaddr;
    char recvline[N];
    sockets(){
        bzero(&servaddr,sizeof(servaddr));
        bzero(&cliaddr,sizeof(cliaddr));
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(sockfd==-1){
            perror("init_sock");
            exit(-1);
        }
    }
    ~sockets(){}
    int initservaddr();
};

int  sockets::initservaddr(){
    int optval=1;    
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int))<0)
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY) ;
    servaddr.sin_port = htons(PORT);
    if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(sockaddr_in))<0){
        perror("bind");
        return -1;
    }

    if(listen(sockfd,20)<0){
        perror("listen error!");
        return -1;
    }

    cout<<"listen succeed....."<<endl;
    for(;;){
        char buff[20];
        len= sizeof(cliaddr);
        connfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);
        if(connfd<0){
            perror("accept error!");
            return -1;
        }
        cout<<"connect from port "<<inet_ntop(AF_INET,&cliaddr,buff,sizeof(buff))<<ntohs(cliaddr.sin_port)<<endl;
        times = time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&times));
        
        if(write(connfd,buff,strlen(buff))<0){
            cout<<"write error!"<<endl;
            return -1;
        }
        if(close(connfd)<0){
            perror("closefd");
            return -1;
        }
    }
    return 0;
       
}

int main()
{
    sockets serv;
    sockets cli;
    serv.initservaddr();


}

