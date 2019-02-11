#include <iostream>
#include<dirent.h>
#include<unistd.h>
#include<stdio.h>
#include"unp.h"
#include<sys/stat.h>
#include<sys/epoll.h>
#include<stdlib.h>
using namespace std;
#define MAXSIZE 2000
int init_listen_fd(int port,int epfd);
void do_accept(int lfd,int epfd);
void http_request(const char *request);
int main(int argc ,char ** argv)
{

    if(argc<3){
        cout<<"usage:./a.out <port><path>"<<endl;
        exit(1);
    }
    
    int port = atoi(argv[1]);
    if(chdir(argv[2])==1){
        cout<<"The dir not exist!"<<endl;
        exit(1);
    }

    return 0;
}
void epoll_run(int port){
    
    int epfd = epoll_create(MAXSIZE);
    //添加监听的节点
    //先天家监听的lfd
    //委托内核检测添加到结点
    struct epoll_event all[MAXSIZE];
    int lfd = init_listen_fd(port,epfd);
    while(1){
        int ret =  epoll_wait(epfd,all,MAXSIZE,-1);
        if(ret==-1){

        }
        for(int i=0;i<ret;i++){
            struct epoll_event*pev=&all[i];
            if(!(pev->events&EPOLLIN)){
                //不是读时间
                continue;
            }
            if(pev->data.fd==lfd){
                //接受请求
                do_accept(lfd,epfd);
            }
            else{
                
                //读出来
            }
        }
    }

}

//接受连接请求
void do_accept(int lfd,int epfd){
      
    struct sockaddr_in cli ;
    socklen_t len=sizeof(cli) ;
    int connfd=accept(lfd,(struct sockaddr*)&cli,&len);
    //打印客户端信息
    int flag =fcntl(connfd,F_GETFL);
    flag =O_NONBLOCK;
    
    struct epoll_event et ;
    et.data.fd = connfd;
    et.events=EPOLLIN|EPOLLET;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&et);
    if(ret==-1){
        
    }
}

int get_line(int sock,char* buf,int size){

    int i =0;
    char c ='\0';
    int n ;
    while((i<size-1)&&(c!='\n')){
        n = recv(sock,&c,1,MSG_PEEK);
        if(n>0){
            if(c=='\r'){
                n = recv(sock,&c,1,MSG_PEEK);
                if((n>0)&&(c=='\n')){
                    recv(sock,&c,1,0);
                }
                else
                    c='\n';
            }
        }
        buf[i]=c;
        i++;
    }
    buf[i]='\0';
    if(n==-1){
        i=-1;
    }
    return i;
}
void do_read(int cfd){
    
    char line[1024];
    //将浏览器法国来的数据独到buf里面
    int len = get_line(cfd,line,sizeof(line)) ;
    if(len==0){

    }
    if(len == -1){
            //关闭套接字，从epoll书上删除套接字
    }
    else{

        char buf[1024]={0};
        while(len){
             
            len = get_line(cfd,buf,sizeof(buf));
        }
    }
    //判断是否为get请求
    if(strncasecmp("get",line,3)==0){
        //匹配成功处理http 请求
    }
    //将浏览器法国来的数据独到buf里面
    http_request(buf);
}

void diconnect(int cfd,int epfd){
    
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,cfd,NULL);
    close(cfd);
}

//解析http 请求
void http_request(const char *request,int cfd){
    
    char method[12],path[1024],protocol[12];
    sscanf(request,"%[^ ]%[^ ]%[^ ]",method,path,protocol);
    printf("Method=%s",method);
    printf("path=%s",path);
    printf("peotocol=%s",protocol);
    //处理path
    char*file = path+1;
    //ru guo mei you zhi ding fang wen de zi yuan mo ren xian shi zi yuan mu lu zhong de nei rong
    //判断是否为目录.
    if(strcmp(path,"/")==0){
        strcpy(file,"./");
    }

    struct stat st;
    int ret= stat(file,&st);
    if(ret==-1){
        printf("404     %d",__LINE__);
        exit(1);
    }
    //目录
    if(S_ISDIR(st.st_mode)){
        
        //fa song toou xin xi
        send_respond_head(cfd,200,"OK","text/html",-1);
        //fa song  mu lu han shu
        send_dir(cfd,file);
    }
    //普通文件
    else if(S_ISREG(st.st_mode)){

        send_respond_head(cfd,200,"OK","text/plain",st.st_size);
        //fa song xiang ying tou
        //fa song wen jian nei rong 
        //fa song wen  jian nei rong
        send_file(cfd,file) ;   
    }
}

void send_dir(int cfd,const char *dirname){
    
    //pin yi ge html ye mian<table> </table>
    //xian cong gen kai shi pin 
    char buf[4096]={0};
    char path[4096]={0};
    sprintf(buf,"<html><head><title>mu lu ming :%s</title></head>",dirname);
    sprintf(buf+strlen(buf),"<body><h1>dang qian mu lu:%s</h1><table>",dirname);
    struct dirent **ptr;
    int nums =scandir(dirname,&ptr,NULL,alphasort);
    for(int i=0;i<nums;i++){
        const char *name = ptr[i]->d_name;
        //zi fu chuan pin jie 
        //pin jie wen jian de wan zheng lu jing
        sprintf(path,"%s/%s",dirname,name);
        struct stat st;
        stat(path,&st);
        //pan duan  shi fou wen wen jian huoz he mu lu 
     
        if(S_ISREG(st.st_mode)){
            sprintf(buf+strlen(buf),"<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",dirname,dirname,(long)st.st_size);
        }
        else if(S_ISDIR(st.st_mode)){
            sprintf(buf+strlen(buf),"<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>",dirname,dirname,(long)st.st_size);
        }
        send(cfd,buf,sizeof(buf),0);
        bzero(buf,sizeof(buf));
    }
    sprintf(buf,"</table></body></html>");
    send(cfd,buf,sizeof(buf),0);
#if 1
    
    //da kai mu lu
    /*
        DIR*dir = opendir(dirname);
        if(dir==NULL){
            printf("open eror!");
            exit(1);
        }
        struct dirent*ptr=NULL;
        while((ptr=readdir(dir))!=NULL){
            const char *name=ptr->d_name;
        }
        closedir(dir);*/
#endif
}

void send_file(int cfd,const char *filename){
    
    int fd = open(filename,O_RDONLY);
    if(fd==-1){
        return ;
    }
    char buf[4096];
    char len = 0;
    while((len=read(fd,buf,sizeof(buf)))>0){
        send(cfd,buf,len,0);
    }

    if(len==-1){
        printf("read file error!");
        exit(1);
    }
    close(fd);
}
//fa song xiang ying tou
void send_respond_head(int cfd,int  num,const char *desp,char *type,long len){
        
char buf[1024]={0};
sprintf(buf,"http/1.1 %d %s\r\n",num,desp);
//zhuang tai hang ,xiao xi bao tou
//kong hang
send(cfd,buf,strlen(buf),0);
sprintf(buf,"Content-type:%s\r\n",type);
send(cfd,buf,strlen(buf),0);
sprintf(buf,"Content-Length:%ld\r\n",len);
send(cfd,buf,strlen(buf),0);

send(cfd,"\r\n",2,0);
//zhuang tai hang ,xiao xi bao tou


}

int init_listen_fd(int port,int epfd){
    
    int fd=socket(AF_INET,SOCK_STREAM,0) ;
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
  //........
    int flag =1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    
    int ret = listen(fd,20);
    //将fd瓜到epoll书上
    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd = fd;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret ==-1){
        
    }
    return fd;
}
