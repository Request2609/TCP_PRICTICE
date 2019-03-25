#include "apue.h"
//竞态条件的程序

static void charatatime(char* strs) ;
int main()
{

    pid_t pid ;

    if((pid = fork()) < 0)
    {
        err_sys("fork error!") ;
    }
    
    else if(pid == 0)
    {
        charatatime("output from child!\n"); 
    }   
    else
    {
        charatatime("output from parents!\n") ;
    }
    exit(0) ;
}

static void charatatime(char* strs)
{
    char* ptr ;
    int c ;
    setbuf(stdout, NULL) ;
    for(ptr = strs; (c = *ptr++) !=0; )
    {
        putc(c, stdout) ;
    }
}
