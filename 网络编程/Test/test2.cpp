#include <iostream>
#include<string.h>
using namespace std ;
int main()
{
    const char * str = "hello world" ;
    cout<<strspn(str,"hellow")<<endl;
    cout<<strpbrk(str," w")<<endl;
    return 0;
}

