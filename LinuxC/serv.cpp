#include <iostream>
#include<string.h>
#include<thread>
#include<fstream>
#include<functional>

#include"socketfd.h"
using namespace std ;
#define N 1024
shared_ptr<sockfd> serv ;
char path[1024] ="server" ;
char ip[1024] ;
char port[1024] ;
void deamon() ;
void new_listen(int sock) ;
void new_connect() ;
void handler(int sig) ;
void process(int connfd) ;
void set_sig() ;

void new_connect()
{
    openlog("myServer",LOG_CONS|LOG_PID, LOG_USER) ;
    serv->s_bind() ;
    serv->s_listen() ;
    int connfd ;
    
    while(1)
    {
        if((connfd = accept(serv->fd, (struct sockaddr*)NULL, NULL)) < 0)
        {
            syslog(LOG_ERR, "accept socket error:%s(errno:%d)", strerror(errno),errno) ;
            exit(1) ;
        }

        thread t1(process ,connfd);
        t1.join() ;
    }
    closelog() ;
}

void process(int connfd)
{
    char buf[1024] ;
    int n ;
    if((n = recv(connfd, buf, sizeof(buf), 0)) < 0)
    {
        syslog(LOG_ERR, "recv error:%s(errno:%d)\n",strerror(errno), errno) ;
    }

    buf[n] = '\0' ;
    ofstream out("/tmp/output", ios::app|ios::out) ; 
    out <<"thread id:" <<this_thread::get_id() <<"----------process id:" <<getpid()<<"    /" <<"function process" <<endl ;
    out.close() ;

}

int main(int argc, char ** argv)
{
    
    strcpy(path, argv[0]) ;
  /*  if(argc == 2)
    {
        strcpy(ip, "127.0.0.1") ;    
        strcpy(port, argv[2]) ;
        serv =  sockfd::getsockfd(ip, argv[1]) ;
        deamon() ;
        new_listen() ;
    }   
    else if(argc == 3)
    {
        strcpy(ip, argv[1]) ;
        strcpy(port, argv[2]) ;
        serv = sockfd::getsockfd(ip, port) ;
    }
    else
    {
        cout << "USAGE:<a.out><ip><port>or<a.out><port>" <<endl ;
        exit(0) ;
    }*/

    if(argc == 1)
    {
        strcpy(ip,"127.0.0.1") ;
        strcpy(port, "2345") ;
        deamon() ;
        serv = sockfd::getsockfd(ip, port) ;
        new_connect() ;
    }
    else if(argc == 2)
    {
        int listenfd = atoi(argv[1]) ;
        set_sig() ;
        new_listen(listenfd); 
    }
}


//创建守护进程
//先fork -> 将父进程退出->设置文件权限
//->创建新的会话组->关闭标准输出，标准输入，
//标准错误输出文件描述符，让服务器为守护进程
void deamon()
{
    pid_t pid ;
    pid = fork() ;
    if(pid < 0)
    {
        cout << "创建进程失败"<<endl ;
        exit(1) ;
    }
    if(pid > 0)
    {
        exit(0) ;
    }
    
    //设置文件权限
    umask(0) ;
    
    setsid() ;
    
    close(STDIN_FILENO) ;
    close(STDOUT_FILENO) ;
    close(STDERR_FILENO) ;
}

void child_listen()
{
    
}

void set_sig()
{
    //记录日志
    openlog("myServer", LOG_CONS|LOG_PID, LOG_USER) ;   
    struct sigaction action ;
    action.sa_flags = 1 ;
    action.sa_handler = handler ;
    if(sigaction(SIGALRM, &action, NULL) < 0)
    {
        syslog(LOG_ERR, "set signal error:%s(errno:%d)",strerror(errno), errno) ;
    }
    else
    {
        syslog(LOG_INFO, "set signal ok...") ;
    }

    closelog() ;
}

//信号处理函数
//收到alrm信号后，进行热重启
void handler(int sig)
{
    openlog("myServer", LOG_CONS|LOG_PID, LOG_USER);
    syslog(LOG_INFO, "process signal...") ;
    pid_t pid ;
    pid = fork() ;
    if(pid < 0)
    {
        cout << "创建新进程失败" << endl ;
        exit(1) ;
    }

    if(pid > 0)
    {
        syslog(LOG_INFO, "父进程退出...") ;
    }

    if(pid == 0)
    {
        char args[1024] ;
        sprintf(args, "%d", serv->fd) ;
        execle(path, args, NULL, environ) ;   
    }
    closelog() ;
}

void new_listen(int sock)
{
    openlog("myServer", LOG_CONS|LOG_PID, LOG_USER) ;
    while(1)
    {
        int connfd ;
        if((connfd = accept(sock, (struct sockaddr*)NULL, NULL)) < 0)
        {
            syslog(LOG_INFO, "new process accept socket error!") ;
            break ;
        }
        if(errno == EINTR)
        {
            syslog(LOG_INFO, "new process accept socket error!") ;
            break ;
        }
        else
        {
            syslog(LOG_INFO, "new process accept  a socket") ;
        }
        auto task = bind(process, connfd) ;
        thread t1(task) ;
    }
    closelog() ;
}
