#include <iostream>
using namespace std;
#include"unp.h"
void process(int connfd){
    
    ssize_t n;
    char buf[MAXLINE];
again:
    while((n=read(connfd,buf,MAXLINE))>0){
        write(connfd,buf,n);
        //cout<<buf<<endl;
        if(n<0&&errno==EINTR){
            goto again;
        }
        else if(n<0){
            cout<<"process read"<<endl;
        }
    }
    
}
int main()
{
    int listenfd ,connfd;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<-1){
        cout<<"socket error!"<<endl;
        exit(-1);
    }
    pid_t pid;
    struct sockaddr_in serv,cli;
    socklen_t len;
    serv.sin_family=AF_INET;      
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    serv.sin_port=htons(PORT);
    if(bind(listenfd,(struct sockaddr*)&serv,sizeof(serv))<-1){
        cout<<"bind error!"<<endl;
        exit(-1);
    }
    if(listen(listenfd,MAXLINE)<-1){
        cout<<"listen error!"<<endl;
        exit(-1);
    }
    while(1){

        len = sizeof(cli);
        connfd = accept(listenfd,(struct sockaddr*)&cli,&len);   
        if(connfd < 0){
                cout<<"accept error!"<<endl;
                exit(-1);
            }
        if((pid =fork())==0){
            close(listenfd);
            process(connfd);
            close(connfd);
            exit(0);
        }
        close(connfd);
    }
    return 0;
}

