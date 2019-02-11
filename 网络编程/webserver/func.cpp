#include"func.h"
mutex mute ;
mutex mute_ ;
//设置套接mutex mute ;字为非阻塞
int setnonblocking( int& fd )
{
    int _old = fcntl ( fd , F_GETFL ) ;
    
    int _new = _old | O_NONBLOCK ; 
    fcntl( fd , F_SETFL , _new );
    
    return _new ;
}


int sock_init( const char * ip,const int& port )
{

    int sock = socket( AF_INET, SOCK_STREAM, 0 ) ;
    assert( sock != -1 );

    int flag = 1 ;
    int ret = setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag) );
    assert( ret >= 0 );

    struct sockaddr_in serv ;
    bzero( &serv, sizeof( serv ) ) ;
    serv.sin_family = AF_INET ;
    serv.sin_port = htons( port ) ;
    inet_pton( AF_INET, ip, &serv.sin_addr );

    ret = bind( sock, ( struct sockaddr* )&serv, sizeof(serv) ) ;
    if( ret < 0 ){

        cout << "bind error!" << endl ;
        exit(1) ;
    }
    
    ret = listen( sock, MAXLINE );
    
    if( ret < 0 ) {

        cout<<"listen exit!"<<endl ;
        exit(1) ;
    }
    return sock ;
}


void run_epoll( int& epfd , epoll_event evfds[] , int sfd)
{
    
    ThreadPool p ;
    p.start( MAXLINE );
    
    //unique_lock<mutex>lk(mute);
    while( 1 )
    {
        int ret = epoll_wait( epfd , evfds , MAX ,-1 );
        if( ret < 0 )
        {
            cout<<"epoll wait exit!"<<endl;
            exit(1) ;
        }

        
        for( int i = 0 ; i < ret ;i++ )
        {
            int sockfd = evfds[i].data.fd  ;
            
            if(sockfd == sfd )
            {
                struct sockaddr_in cli ;
                socklen_t len = sizeof( cli ) ;

                int connfd = accept( sfd , ( struct sockaddr* )&cli , &len );
                if( connfd < 1 )
                {
                    cout << "accept error!" <<endl ;
                    exit(1) ;
                }
                
                cout<<"New Connection------------------------>"<<endl;
                setnonblocking(connfd);
                add_fd( epfd , connfd);
            }

            else if ( evfds[i].events & EPOLLIN )
            {

                process( sockfd , epfd ,p );
            }

            else
            {
                cout<< "somthing else is happen!"<<endl ;
                break ; 
            }
        }
    }
}


void add_fd(int& epfd ,int& sfd)
{
    struct epoll_event ev ;
    ev.data.fd = sfd ;
    ev.events = EPOLLIN | EPOLLET ;
    int ret = epoll_ctl( epfd , EPOLL_CTL_ADD , sfd, &ev ) ;
    if ( ret < 0 )
    {
        cout << "add epoll fd error!" << endl;
        exit(1) ;
    }
}

void process( int& connfd , int& epfd ,ThreadPool& p)
{

    auto task = bind( do_read , connfd ,epfd );
    p.addTask( task ) ;
}


void do_close( int& connfd , int& epfd )
{
    
    int ret = epoll_ctl( epfd, EPOLL_CTL_DEL ,connfd, NULL ) ;
    if( ret == -1 ) 
    {
        cout<<"delete exit!"<<endl ;
        exit(1);
    }

    close(connfd) ;
}   

int  get_line( int& sock , char* line ,int& len)
{
    
    int i = 0 ;
    char c = '\0' ;
    int n  ;

    while( (i < len - 1)&&(c != '\n'))
    {

        n = recv(sock , &c , 1 , 0 ) ;
        if( n == 0 )
        {
            break ;
        }

            if( n > 0 )
            {
                if( c == '\r' )
                {
                    n = recv( sock , &c , 1 , MSG_PEEK ) ;
                    if( n > 0 && c == '\n')
                    {
                        recv(sock , &c , 1 , 0);
                    }
                    else
                    {
                        c = '\n' ;
                    }
                }
                line[i] = c ;
                i++ ;
            }
            else
            {
                c = '\n'  ;  
            }
    }

    line[i] = '\0' ;
    if( n == -1 )
    {
        i = -1 ;
    }

    return i ;  
}

void do_read( int& connfd , int& epfd )
{
    
    char line[MAX] ;
    bzero( line , sizeof(line) );
    int s = sizeof(line) ;   
    int len = get_line( connfd , line , s );
    
    if( len == 0 )
    {
        do_close(connfd , epfd);
        return ;
    }

    if(len == -1)
    {
        cout<<"recv error!"<<endl ;
        return ;
    }
    
    else{
        cout<<"请求行数据:"<<endl ;
        cout<<line<<"";
        while(len)
        {
            if(len == -1)
            {
                break ;
            }
            char buf[MAX] = {0};
            int s =sizeof(buf) ;
            len = get_line(connfd , buf , s) ;
            cout<<buf<<endl ;
        }
    }
    
    if(strncasecmp("get" , line ,3 ) == 0 )
    {
        http_request( line , connfd ) ;
        do_close( connfd ,epfd ) ;
    }
}

void send_response_head(int connfd , int nu , const char* ss , const char * type , long l)
{
    char buf[1024] ;
      
    //状态行
    sprintf(buf ,"http/1.1 %d %s\r\n" ,nu ,ss ) ;
    cout<<buf<<endl ;
    send( connfd , buf , strlen(buf), 0 );

    //消息包头
    cout<<"Type:"<<type<<endl;
    sprintf(buf , "Content-Type: %s\r\n",type) ;
    send( connfd , buf , strlen(buf) , 0);
    sprintf(buf+strlen(buf) , "Content-Length: %ld\r\n",l) ;
    send(connfd , buf ,strlen(buf) ,0) ;
    cout<<"length:"<<l<<endl;
    sprintf(buf,"\r\n") ;
    send(connfd , buf , 2, 0) ;
}

void http_request(const char* buf ,int connfd )
{
    char name[50];    
    getcwd(name,sizeof(name));;
    cout<<name<<endl;

    char method[MAX] ;
    char proctol[MAX] ;
    char _path[MAX] ;
    
    sscanf(buf ,"%s %s %s",method, _path , proctol );
    
    //主要是path
    decode_str( _path , _path ) ;
    char* file = _path + 1 ;
    
    if( strcmp(_path , "/") == 0)
    {
        strcpy( _path , "1.html") ;

        struct stat st ;
        int ret = stat(_path , &st) ;
        if( ret == -1 )
        {
            
            send_error(connfd) ;
            return ;
        }
        
        send_response_head( connfd , 200 , "OK" , get_file_type(_path) , st.st_size);
        send_file( _path , connfd );
    }

    else
    {
        cout<<file <<endl;  
        struct stat st ;
        int ret = stat(file , &st) ;
        if( ret == -1 )
        {
            send_error(connfd) ;
            return ;
        }
        send_response_head( connfd , 200 , "OK" , get_file_type(file) , st.st_size);
        send_file(file , connfd );
    }    
}

void send_error( int connfd )
{

    send_response_head(connfd , 404 , "File Not found",".html",-1);;
    send_file("404.html",connfd);
    return ;

}

void send_file(const char* file , int connfd)
{
        cout<<"  filename"<<file<<endl;
        int fd = open( file , O_RDONLY ) ;
        
        if(fd == -1)
        {
            return  ;
        }

        char buf[4096] ;
        //char buf1[4104];
        int len = 0 ;
        while( (len = read(fd ,buf ,sizeof(buf))) > 0 )
        {
           send( connfd , buf ,len , 0) ;
        }
       
        if( len == -1 )
        {
            perror("read file error!") ;
            exit(1) ;
        }
        close(fd) ;
        cout<<"Send OK"<<endl;
}  

//编码
void encode_str( char *to , int tosize ,const char *from ){

    int tolen ;

    for( tolen = 0 ;*from!='\0'&&tolen+4<tosize ;++from ){

        if(isalnum(*from) || strchr ("/_.-~",*from)!=(char*)0){
            *to = *from ;
            ++to ;
            ++tolen ;
        }
        else{
            //2位十六进制数不够
            sprintf(to ,"%%%02x",(int)*from&0xff);

            to+=3 ;
            tolen +=3 ;
        }
    }
    *to = '\0' ;
}

//解码
void decode_str(char *to ,char *from ){

    for( ; *from!='\0' ;++to,++from){

        if(from[0] =='%'&& isxdigit(from[1])&&isxdigit(from[2]))
        {
            *to = hexit(from[1])*16 +hexit(from[2]) ;
            from+=2 ;
        }
        else{
            *to = *from ;
        }
    }
    *to = '\0' ;
}

int hexit(char c){

    if(c>='0'&&c<='9')
        return c-'0' ;
    if(c>='a'&&c<='f')
        return c-'a'+10 ;
    if(c>='A'&&c<='F')
        return c-'A'+10 ;
    return 0 ;
}

const char* get_file_type(char* name){

    char *dot ;

    //自右向左查找.字符,如果不存在.,返回NULL
    dot = strrchr(name ,'.');   
    cout<<dot<<"**************"<<endl;
    if(dot == NULL)
        return "text/plain; charset=utf-8";
    if(strcmp(dot ,".html")==0||strcmp(dot,".htm")==0)
        return "text/html; charset=utf-8";
    if(strcmp(dot,".jpg")==0)
        return "image/jpg";
    if(strcmp(dot,".jpeg") == 0)
        return "image/jpeg";
    if(strcmp(dot,".gif"))
        return "image/gif";
    if(strcmp(dot,".png")==0)
        return "image/png";
    if(strcmp(dot,"text/css")==0)
            return "text/css";
    if(strcmp(dot,".au")==0)
        return "audio/basic" ;
    if(strcmp(dot,".wav")==0)
        return "audio/wav";
    if(strcmp(dot,".mpeg")==0||strcmp(dot,"mpe")==0)
        return "video/mpeg";
    if(strcmp(dot,".vrml")==0||strcmp(dot,"wrl")==0)
        return "model/vrml";
    if(strcmp(dot,".mp3")==0)
        return "audio/mpeg";
    if(strcmp(dot,"ogg")==0)
        return "application/ogg";
    if(strcmp(dot,".pac")==0)
        return "application/x-ns-proxy-autoconfig";
    return "text/plain; charset=utf-8";
}
