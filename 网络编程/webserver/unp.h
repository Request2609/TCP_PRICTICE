/*************************************************************************
	> File Name: unp.h
	> Author: 
	> Mail: 
	> Created Time: Sun 18 Nov 2018 12:55:40 AM PST
 ************************************************************************/

#ifndef _UNP_H
#define _UNP_H
#include<iostream>
#include<netinet/in.h>
#include<time.h>
#include<assert.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<netdb.h>
#include<sys/uio.h>
#include<sys/stat.h>
#include<sys/un.h>
#include<signal.h>
#include<sys/epoll.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/sysctl.h>
#include<stdarg.h>
using namespace std;
#define LISTENQ 100
#define PORT  2345
#define MAXLINE 20
#define MAX 1024
/*int tcp_connect(const char*host,const char *serv);

int tcp_connect(const char *host,const char*serv){

    int sockfd,n;
    struct addrinfo hints,*res,*ressave;
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if((n=getaddrinfo(host,serv,&hints,&res))!=0){
        printf("tcp_connect:error for %s,%s,%s",host,serv,gai_strerror(n));
        exit(1);
    }
    ressave = res ;
    do{

        sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(sockfd<0){
            cout<<"socket error!"<<endl;
            continue;
        }
        if(connect(sockfd,res->ai_addr,res->ai_addrlen)==0){
            break;
        }

        close(sockfd);
        
    }while((res=res->ai_next)!=NULL);
    
    if(res==NULL){
        cout<<"tcp_connect error!"<<endl;
        exit(1);
    }
    freeaddrinfo(ressave);
    return sockfd;
}

int tcp_listen(const char *host,const char*serv,socklen_t *addrlenp){

    int listenfd,n;
    const int on = 1;
    struct addrinfo hints,*res,*ressave;
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_flags=AI_PASSIVE;
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    if((n=getaddrinfo(host,serv,&hints,&res))!=0){
        cout<<"tcp_listen "<<"line:"<<__LINE__<<endl;
        exit(1);
    }
    ressave = res;
    do{
        listenfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        if(listenfd<0){
            continue ;
        }
        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
        if(bind(listenfd,res->ai_addr,res->ai_addrlen)==0){
            break;
        }
        close(listenfd);
    }while((res=res->ai_next)!=NULL);
    if(res==NULL){
        cout<<"tcp_listen"<<"    Line:"<<__LINE__<<endl;
        exit(1);
    }
    listen(listenfd,LISTENQ);
    if(addrlenp){
        *addrlenp = res->ai_addrlen;
    }
    freeaddrinfo(ressave);
    return listenfd;
}

*/
#endif

