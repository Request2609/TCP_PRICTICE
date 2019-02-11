#include <iostream>
#include<map>
using namespace std;
bool cmpfunc(char a,char b){
    return a<b;
}

class compared{
    
    bool operator()(int a,int b){
        return a>b;
    }
};

int main()
{
    //定义
    map<char,int> mymap ;
    mymap['a'] =10;
    mymap['b']=11;
    //复制
    map<char,int>second(mymap);
     
    return 0;
}

