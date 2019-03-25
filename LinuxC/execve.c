#include<unistd.h> 
#include"sys.h"

//切换进程
int main()
{

    char * args[] = { "bin/ls" , NULL } ;
    printf("系统分配ID = %d" ,getpid() ) ;
    if(execve( "/bin/ls", args, NULL ) < 0 )
    {
        printf( "创建进程失败" ) ;
    }


    return 0;
}

