#include <iostream>
#include"apue.h"

using namespace std ;

static void lockbyte(const char* name, int fd, off_t offset)
{
    //加写锁
    if(writew_lock(fd, offset, SEEK_SET, 1) <1)
    {   
        cout << ""
    }
}
int main()
{
    std::cout << "Hello world" << std::endl;
    return 0;
}

