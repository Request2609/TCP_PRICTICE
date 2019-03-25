#include"process.h"

process :: process( int connfd, int servfd)
{
    this -> connfd = connfd ;
    this -> servfd = servfd ;
    m_read_buffer[0] = '\0' ;
    do_read() ;
}


process :: ~process(  )
{
    close( connfd ) ;
    m_read_buffer[ 0 ] = '\0' ;
}

void process :: do_read()
{
    int m_read_idx ;
    int bytes_read = 0 ;
    while( true )
    {
        bytes_read = recv( connfd, m_read_buffer + bytes_read, )
    }
}
