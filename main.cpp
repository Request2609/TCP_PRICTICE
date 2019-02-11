#include <iostream>
#include<strings.h>
using namespace std;
#include"unp.h"
void http_response(int cfd ,const char *type){

    char buf[1024];

    sprintf(buf,"http/1.1 200 OK\r\n");
    write(cfd,buf,(sizeof(buf)));
    sprintf(buf,"Content-Type:%s\r\n",type);
    write(cfd,buf,sizeof(buf));
}
void getinfo(char *buf,char *method,char*paths,char*proctol){

    int j=0;
    int flag =0;
    for(unsigned int i=0;i<strlen(buf);i++){
        if(flag == 0){
            method[j]=buf[i];
            if(method[j]==' '){
                method[j]='\0';
                flag++;
                j=0;
                continue;
            }

        }
        if(flag ==1){
            paths[j]=buf[i];
            if(paths[j]==' '){
                paths[j]='\0';
                flag++;
                j=0;
                continue;
            }
        }
        if(flag==2){
            proctol[j]=buf[i];
            if(proctol[j]=='\r'){
                proctol[j]='\0';
                flag ++;
                j=0;
                continue;
            }
        }
        if(flag==3){
            break;
        }

    }
}
int main()
{
    
    chdir("path");
    int sockfd ,connfd;
    socklen_t len;
    struct sockaddr_in serv,cli;
    len = sizeof(cli);
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    serv.sin_family = AF_INET;
    serv.sin_port=htons(PORT);
    serv.sin_addr.s_addr= INADDR_ANY;
    int ss =bind(sockfd,(struct sockaddr*)&serv,sizeof(serv));
    if(ss<0){
        cout<<"bind error!"<<"in"<<"    "<<__LINE__<<endl;
        exit(1);
    }
    if((ss=listen(sockfd,MAXLINE))<0){
        cout<<"listen error!"<<"  "<<__LINE__<<endl;
        exit(1);
    }
    if((connfd =accept(sockfd,(struct sockaddr*)&cli,&len))<0){
        cout<<"accept error!"<<"    "<<endl;
        exit(1);
    }
    
    char buf[1024];
    read(connfd,buf,sizeof(buf));

    char method[12],paths[1024],proctol[12];
    
    bzero(method,sizeof(method));
    bzero(paths,sizeof(paths));
    bzero(proctol,sizeof(proctol));
    getinfo(buf,method,paths,proctol);
    
    int fd = open(paths,O_RDONLY);
    const char * p = "text/plain";
    http_response(connfd,p);
    while(read(fd,buf,100)>0){
        write(connfd,buf,sizeof(buf));
    }
    return 0;
}

