#pragma once
#include"unp.h"
#include"ThreadPool.h"

int sock_init(const char* ip , const int& port) ;
int setnonblocking( int& fd );
void add_fd( int& epfd , int& sfd );
void run_epoll( int& epfd , epoll_event evfds[] , int sfd );
void do_read( int& connfd ,int& epfd ) ;
void process( int& connfd , int& epfd ,ThreadPool& p ) ;
void do_close( int& connfd ,int& epfd ) ;
int get_line( int& connfd, char* line , int& len) ;
void http_request( const char *buf , int connfd ) ;
void send_file(const char* file , int connfd) ;
const char* get_file_type(char *name);
void send_response_head(int connfd , int nu , const char* ss , const char *type ,long l);
int hexit(char c) ;
void send_error(int connfd);
void decode_str(char *to ,char *from );
void encode_str( char *to , int tosize ,const char *from );
