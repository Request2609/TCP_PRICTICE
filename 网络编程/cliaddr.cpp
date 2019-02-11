#include <iostream>
#include<stdio.h>
#include"unp.h"
using namespace std;
static int read_cnt;
static char*read_ptr;
static char read_buf[MAXLINE];
ssize_t my_read(int fd,char* c){    
    if((read_cnt<0)){
        again:
        if((read_cnt=read(fd,read_buf,sizeof(read_buf)))<0){
            if(errno==EINTR)
                goto again;
            return -1;
        }else if(read_cnt==0){
                return 0;
            }
            read_ptr = read_buf;
        }
      read_cnt--;
      *c =*read_ptr++;
      return 1;
}
int readline(int fd,char *vptr,int maxlen){
      ssize_t n,rc;
      char c, *ptr;
      *vptr ='\0';
      ptr = vptr ;
      for(n=1;n<maxlen;n++){
        
          if((rc=my_read(fd,&c))==1)  {
    
              *ptr++ =c ;
          if(c=='\n')
                break;
             }
          else if(rc==0)  {
              *ptr =0;
              return n-1;
          }
          
          else return -1;
    }

      *ptr=0;
      return n;
}
void sendinfo(FILE*fp,int sockfd){
    
    char recvline[MAXLINE];
    char sendline[MAXLINE];
    cout<<"hello"<<endl;
    while(fgets(sendline,MAXLINE,fp)!=NULL){
        if(write(sockfd,sendline,strlen(sendline))<0){
            cout<<"write error!"<<endl;
            exit(-1);
        }
        if(readline(sockfd,recvline,MAXLINE)==0){
            cout<<"readline error!"<<endl;
            return ;
        }
        cout<<recvline<<endl;
        if(fputs(recvline,stdout)<0){
            cout<<"fputs error!"<<endl;
            exit(-1);
        }
    }
}



void str_cli(FILE *fp , int sockfd){

    int maxfd,s;
    fd_set rset ;
    char buf[MAXLINE];
    int n;
    s=0;
    //清空描述符
    FD_ZERO(&rset);
    for(;;){
        if(s == 0){
            FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
    maxfd =max(fileno(fp),sockfd)+1;
    select(maxfd,&rset,NULL,NULL,NULL);
    if(FD_ISSET(sockfd,&rset)){
        if((n=read(sockfd,buf,MAXLINE))==0){
            if(s==1){
                return ;
            }
     
            else
                cout<<"error!"<<endl;
        }
        write(fileno(stdout),buf,n);
    }
    if(FD_ISSET(fileno(fp),&rset)){
        
        if((n=read(fileno(fp),buf,MAXLINE))==0){
            s =1;
            shutdown(sockfd,SHUT_WR);
            FD_CLR(fileno(fp),&rset);
            continue;
        }
        write(sockfd,buf,n);
       }
    }
}
}
int main(int argc,char** argv){
    int sockfd ;
    struct sockaddr_in cli;
    if(argc!=2){
        cout<<"currect using:./a.out <IP>"<<endl;
    }
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd <0){
        cout<<"socket error!"<<endl;
        exit(-1);
    }
    bzero(&cli,sizeof(cli));
    cli.sin_family = AF_INET;
    cli.sin_port=htons(PORT);
    if(inet_pton(AF_INET,argv[1],&cli.sin_addr)<-1){
        cout<<"Inet_Pton error~!"<<endl;
        exit(-1);
    } 
    if(connect(sockfd,(struct sockaddr*)&cli,sizeof(cli))<0){
        cout<<"connect error!"<<endl;
        exit(-1);
    }
    exit(0);
}

