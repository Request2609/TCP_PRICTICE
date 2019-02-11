#include <iostream>
using namespace std;
#include"unp.h"
void str_cli(FILE *fp,int sockfd){
    
    int maxfdp1,val,std;
    ssize_t n,nwrite;
    fd_set rset,wset;
    char to[MAXLINE],fr[MAXLINE];
    char *toiptr,*tooptr,*friptr,*froptr;
    val = fcntl(sockfd,F_GETFL,0);
    if(val < 0){
        exit(-1);
    }

    
}
