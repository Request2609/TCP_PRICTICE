#include <iostream>
#include<thread>
using namespace std  ;
int func(int a, int b)
{
    cout<<a<<"---------------"<<b<<endl ;
    return 1 ;
}
using namespace std ;
int main()
{
    int a= 1,b=2 ;
    thread t(func,a,b);
    t.join();
    std::cout << "Hello world" << std::endl;
    return 0;
}

