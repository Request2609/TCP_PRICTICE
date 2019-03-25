#include"apue.h"
int main()
{
    pid_t pid ;
    
    if((pid = fork())<0)
    {
        err_sys("fork error!") ;
    }
    
    else if(pid == 0)
    {   
        if((pid = fork())<0)
        {
            err_sys("fork error!") ;
        }
        else if(pid>0)
        {
            exit(0) ;
        }
        sleep(2) ;
        cout << "second child,parent pid = "<< (long)getppid()<< endl ;
        exit(0) ;
    }

    if(waitpid(pid, NULL, 0)!=pid)
    {
        err_sys("waitpid error!") ;
    }
    exit(0) ;
}

