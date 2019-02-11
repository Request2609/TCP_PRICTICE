#include <iostream>
#include"unp.h"
using namespace std;
int main()
{
    int listenfd,connfd;

    socklen_t len ;
    struct sockaddr_in servaddr,cliaddr;
    char buff[MAXLINE];
    time_t ticks;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd <0){

        cout<<"listened....."<<endl;
        exit(-1);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
        cout<<"bind error!"<<endl;
        exit(-1);
    }
    if(listen(listenfd,20)<0){
        cout<<"listen error~!..."<<endl;
        exit(-1);
    }

    for(;;){
        len = sizeof(cliaddr);
        connfd = accept(listenfd,(struct sockaddr*)&servaddr,&len);
        if(connfd < 0){
            cout<<"accept error!"<<endl;
            exit(-1);
        }
        ticks = time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
        if(write(connfd,buff,strlen(buff))<0){
            cout<<"write error!"<<endl;
            exit(-1);
        }
        
        close(connfd);
    }
}

