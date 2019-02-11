#include"unp.h"

//读缓冲区的大小
#define BUFFER_SIZE 4096

//主状态机的两种可能状态,分别表示:读取到一行完整的行,行出错和行数据尚且不完整
enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0 ,CHECK_STATE_HEADER = 1};

//从状态机的三种可能状态,行读取状态
//分别表示:读取到一个完整的行,行出错,行数据尚且不完整
enum LINE_STATUS { LINE_OK = 0 , LINE_BAD , LINE_OPEN };

//服务器处理http请求结果
//NO_REQUEST 表示请求不完整,需要继续读取客户数据;
//GET_REQUEST 表示获得了完整的客户请求;
//BAD_REQUEST  表示客户请求有语法错误;
//FORBIDDEN_REQUEST  表示客户对资源没有访问权限
//INTERNEL_ERROR  服务器内部出现错误
//CLOSE_CONNECTION  表示客户端已经关闭连接
enum HTTP_CODE { NO_REQUEST , GET_REQUEST ,BAD_REQUEST , FORBIDDEN_REQUEST , INTERNAL_ERROR ,CLOSED_CONNECTION };

static const char* sizet[] = { "I get a correct result \n" ,"Something wrong\n" } ;

//用于解析出一行内容
LINE_STATUS parse_line(char * buffer , int & checked_index , int & read_index);
HTTP_CODE parse_requestline( char * temp ,CHECK_STATE& checkstate );
HTTP_CODE parse_headers( char* temp );
HTTP_CODE parse_content( char * buffer , int & check_index , CHECK_STATE& checkstate ,int& read_index ,int & start_line);

int main( int argc ,char ** argv)
{

    if ( argc <= 2)
    {
        cout << "usage :name <ip><port>" << endl ;
        return 0 ;
    }

    const char * ip = argv[1] ;
    int port = atoi( argv[2] ) ;

    struct sockaddr_in  address ;
    bzero( &address ,sizeof(address) ) ;
    address.sin_family = AF_INET ;
    inet_pton(AF_INET , ip , &address.sin_addr );
    address.sin_port = htons( port ) ;
    
    int listenfd = socket( AF_INET, SOCK_STREAM , 0) ;
    assert(listenfd != -1) ;
    int ret = bind(listenfd , (struct sockaddr *)&address , sizeof( address )) ;
    assert( ret != -1 );
    ret = listen( listenfd, 5 ) ;
    assert( ret != -1 );
    struct sockaddr_in client_addr ;
    socklen_t len = sizeof(client_addr) ;
    
    int fd = accept( listenfd, (struct sockaddr*)&client_addr ,&len) ;

    assert( fd >= 0 );
    char buffer[BUFFER_SIZE] ;
    bzero( buffer , sizeof(buffer)) ;
    int data_read = 0 ;
    int check_index = 0 ;
    int read_index = 0 ;
    int start_line = 0 ;
    CHECK_STATE  checkstate = CHECK_STATE_REQUESTLINE ;

    while( 1 )
    {
        data_read = recv(fd , buffer + read_index , BUFFER_SIZE - read_index , 0); 
        assert(data_read != -1) ;
        if(data_read == 0)
        {
            cout << " remote client has close the connection!" << endl ;
            close(fd) ;
            break ;
        }

        read_index += data_read ;
        
        //分析获得的客户数据
        HTTP_CODE  result = parse_content(buffer , check_index ,checkstate , read_index ,start_line) ;
        if ( result == NO_REQUEST )
        {
            continue ;
        }
        else if (result == GET_REQUEST )
        {
    //        send( fd, szret[0], strlen(szret[0]) ,0) ;
            break ;
        }
        else
        {
        //    send( fd ,szret[1] , strlen(szret[1]) , 0) ;
            break ;
        }
    }
    close(fd) ;
    close( listenfd ) ;
    return 0;
}


//buffer指向应用程序的读缓冲区
//checked_index指向当前正在分析的字节
//read_index指向buffer中客户数据尾部的下一个字节
LINE_STATUS parse_line(char * buffer , int & checked_index , int & read_index)
{

    char temp ;
    for ( ; checked_index < read_index ; ++checked_index )
    {   

        temp = buffer[checked_index] ;
        
        if ( temp == '\r' )
        {
            
            if ( checked_index+1 == read_index )
            {
                return LINE_OPEN ;
            }

            else if ( buffer[ checked_index + 1 ] == '\n' )
            {
                buffer[ checked_index++ ] ='\0' ;
                buffer[ checked_index++ ] = '\0' ;
                return LINE_OK ;
            }
            
            return LINE_BAD ;
        }

        else if ( temp == '\n' )
        {   
            if( ( checked_index > 1 ) && buffer[ checked_index - 1 ] == '\r' )
            {
                buffer[ checked_index-1 ] = '\0' ;
                buffer[ checked_index++ ] = '\0' ;
                return LINE_OK ;
            }
            return LINE_BAD ;
        }
    }

    return LINE_OPEN ;
}

//分析请求行
HTTP_CODE parse_requestline( char * temp ,CHECK_STATE& checkstate )
{
    
    //在temp中找第一次出现空白和\t字符的位置,要是没有就是不合法的url
     char * url =  strpbrk( temp , " \t" ) ;

     if ( !url )
     {
            return BAD_REQUEST ;
     }
     *url++ = '\0';  

     char * method = temp ;

     if ( strcasecmp( method , "GET" ) == 0 )
     {
        cout<<"The request is GET " << endl ;  
     }

     else
     {
            return BAD_REQUEST ;
     }

     url += strspn( url , " \t" ) ;
     char *version = strpbrk(url , " \t") ;

     if ( ! version )
     {
        return BAD_REQUEST ;   
     }

     *version++= '\0' ;
     version += strspn( version , " \t" );
     
     //仅支持http/1.1
     if ( strcasecmp( version, "http/1.1" ) != 0 )
     {
         return BAD_REQUEST ;
     }

     //检查url是否合法
     if ( strncasecmp( url , "http://" ,7 ) == 0)
     {
        
         url += 7 ;
         //串url中首次出现/的位置
         url = strchr (url , '/');
     }

     if( ! url || url[0] != '/')
     {
         return BAD_REQUEST ;
     }

     cout<< "The request URL is :"<<url<<endl ;
     checkstate = CHECK_STATE_HEADER ;
     return NO_REQUEST ;
}

//分析头部字段
HTTP_CODE parse_headers( char* temp )
{

   
    //遇到一个空行,说明是正确的http请求
    cout<<temp<<endl;
    if ( temp[ 0 ] == '\0' )
    {
        return GET_REQUEST ;
    }

    else if (strncasecmp( temp , "Host:" , 5) == 0 )
    {
        temp += 5 ;
        temp += strspn( temp , " \t" ) ;
        cout<<"the request host is :"<<temp<<endl;
    }
    else
    {
        cout<<"I can't handle this header"<<endl ;
    }
    return NO_REQUEST ;
}

//分析http请求的入口函数
HTTP_CODE parse_content( char * buffer , int & check_index , CHECK_STATE& checkstate ,int& read_index ,int & start_line)
{
    //记录当前行的读取状态
    LINE_STATUS linestatus = LINE_OK ;
    //记录http 请求的处理结果
    HTTP_CODE retcode = NO_REQUEST ; 
    //主状态机,用于从buffer中取出所有完整的行
    while( ( linestatus = parse_line( buffer, check_index, read_index ) ) == LINE_OK )
    {
        char *temp =buffer + start_line ;
        start_line = check_index ;

        switch( checkstate )
        {
            case CHECK_STATE_REQUESTLINE :
                retcode =parse_requestline( temp, checkstate ) ;
                if ( retcode == BAD_REQUEST )
                {
                    return BAD_REQUEST ;
                }
                break ;
            case CHECK_STATE_HEADER :
                
                retcode =  parse_headers( temp ) ; 
                if (retcode == BAD_REQUEST )
                {
                    return BAD_REQUEST ;                
                }

                else if (retcode == GET_REQUEST )
                {
                    return GET_REQUEST ;
                }

                break ;

            default:
                return INTERNAL_ERROR ;
        }

    }

    if ( linestatus == LINE_OPEN )
    {
        return NO_REQUEST ;
    }

    else 
    {
        return BAD_REQUEST ;
    }
}


