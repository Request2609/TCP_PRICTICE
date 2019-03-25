#pragma once
#include"unp.h"

using namespace std ;
#define N 1024

class process
{

pivate:
    int servfd ;
    int connfd ;   
    char m_read_buffer[N] ;
public:
    process( int connfd, int servfd ) ;
    ~process() ;
    do_read() ;
private:

};

