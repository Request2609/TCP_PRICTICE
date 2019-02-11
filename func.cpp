#include <iostream>
#include<functional>
#include<vector>
using namespace std;

int c_func(int x,int y){
    return x+y;
}

//通过运算符重载
class functor{

public:
    int operator()(int x,int y){
        return x+y;  
    }
};

int main()
{
    //函数指针的定义
    //可用F定义参数为两个int型变量返回值的int型的函数
    typedef int(*F)(int,int);
    F  f=c_func;
    cout<<f(4,5)<<endl;
    cout<<"----------------------------------------------------\n";
        
    functor ft ;
    cout<< ft(1,2)<<endl;
    //定义function 对象,()里面int为该函数对象的参数,外面int是函数对象的返回值
    function<int(int,int)>myfunc;
    
    //将相同参数的函数可赋值给函数对象myfunc,以后可以通过使用myfunc来调用c_func
    //可以看出这个功能和函数指针相似
    myfunc =c_func;
    cout<<myfunc(3,4)<<endl;
    myfunc = ft ;
    cout<<myfunc(4,6)<<endl;

    //将lambda表达式直接赋值给function对象
    myfunc = [](int a,int b){
        return a+b;
    };

    cout<<myfunc(4,8)<<endl;
    cout<<"--------------------------------------------------"<<endl;
    
    //定义vector容器存指定参数和该值返回值类型的函数
    vector<function<int(int,int)>>v ;
    
    //通过functiond对象传元素
    v.push_back(c_func);

    //通过函数指针传元素
    v.push_back(f);

    //通过运算符重载对象传元素
    v.push_back(ft);

    //通过lambda表达式传元素
    v.push_back([](int a,int b){return a+b;});

    for(const auto&e:v){
        cout<<e(9,7)<<endl;
    }
}


