#include <iostream>
using namespace std;
template<typename T>
void print(T& t){
    cout<<t<<endl;
    cout << "lvalue" << endl;
}
template<typename T>
void print(T&& t){
    cout<<t<<endl;
    cout << "rvalue" << endl;
}
 
template<typename T>
void TestForward(T && v){
    print(v);
    print(std::forward<T>(v));
    //右值转发
    print(std::move(v));
    cout<<"-------"<<endl;
}
 
int main(){
    //
    TestForward(1);
    int x = 1;
    TestForward(x);
    TestForward(std::forward<int>(x));
    return 0;
}

