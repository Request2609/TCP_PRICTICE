#include <iostream>
#include<unistd.h>
#include"unp.h"
using namespace std;
int main()
{
    int i ,maxi,maxfd,listenfd,connfd,sockfd;
    int nready,client[FD_SETSIZE];
    ssize_t n;
    fd_set rset,allset;
    char buf[MAXLINE] ;
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;
    listenfd =socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
        cout<<"socket"<<endl;
        exit(-1);
    }
    bzero(&servaddr,sizeof(sockaddr));
    servaddr.sin_family =AF_INET;
    servaddr.sin_addr.s_addr =htonl(INADDR_ANY);
    servaddr.sin_port=htons(PORT);
    if(listen(listenfd,MAXLINE)<-1){
        cout<<"listen"<<endl;
        exit(-1);
    }
    cout<<"listen..."<<endl;
    maxfd = listenfd;
    maxi = -1;
    for(i=0;i<FD_SETSIZE;i++){
        client[i]=-1;
    }
    //初始化描述符集唯恐
    FD_ZERO(&allset);
    //将监听描述符加入到集合中
    FD_SET(listenfd,&allset);
    for(;;){
        rset = allset;
        nready = select(maxfd+1,&rset,NULL,NULL,NULL);
        //测试指定的文件描述符是否在该集合中
        if(FD_ISSET(listenfd,&rset)){
            clilen = sizeof(sockaddr);
            connfd =  accept(listenfd,(sockaddr*)&cliaddr,&clilen);
            if(connfd<0){
                cout<<"accept"<<endl;
                exit(-1);
            }
            for(i=0;i<FD_SETSIZE;i++){
                if(client[i]==-1){
                    client[i]=connfd;
                    break;
                }
                if(i==FD_SETSIZE){
                    perror("too many fd");
                    exit(-1);
                }
                //将客户套接子村在描述符集合中
                FD_SET(connfd,&allset);
                if(connfd>maxfd){
                    maxfd=connfd;
                }
                //重置套接子数组的长度
                if(i<maxi)maxi=i;
                //判断描述符集合中的就绪描述符数目是否大于等于0
                if(--nready<=0){
                    continue;
                }
            }
         }
            for(i=0;i<=maxi;i++){
                if((sockfd = client[i])<0){
                    continue;
                }
                //检查描述符集合，测试描述符是否在集合中
                if(FD_ISSET(sockfd,&rset)){
                    if((n=read(sockfd,buf,MAXLINE+1))==0){
                        close(sockfd);
                        FD_CLR(sockfd,&allset);
                        client[i]=-1;
                    }else{
                        write(sockfd,buf,MAXLINE+1);
                    }
                    if(--nready<=0)break;
                }
            }
        }
}

