#include <iostream>
#include <functional>
#include <iostream>

//一个自定义的整数类
struct integer{
      int i;
      integer(int i):i(i){}
      void incr_by(int j){ i+=j;}
};

void mem_func_sample(){
     integer x(0);    //x.i=0;
     integer *y=&x;   //y指向x
     using namespace std::placeholders;
     using namespace std;
     
     auto f0=bind(&integer::incr_by,_1,_2);
     f0(x,2);     //x.incr_by(2)
     cout<<x.i<<endl;
     
     f0(y,2);         //y->incr_by(2)
     cout<<x.i<<endl;   //x.i=4
     
     auto f1=bind(&integer::incr_by,x,_1);
     f1(2);   //x在f1的副本.incr(2)
     cout<<x.i<<endl;  x.i=4;
     
     auto f2=bind(&integer::incr_by,ref(x),_1);
     f2(2);   //x.incr_by(2)
     cout<<x.i<<endl;       //x.i=6;
     
     auto f3=bind(&integer::incr_by,&x,_1);
     f3(2);   //(&x)->incr_by(2)
     cout<<x.i<<endl;  //x.i=8
     
     //利用mem_fn将成员函数转换为函数对象
     auto f4=mem_fn(&integer::incr_by);
     f4(x,2);  
     cout<<x.i<<endl;//x.i=10;
     f4(y,2);   
     cout<<x.i<<endl;//x.i=12
     
     
}

int main(){

     mem_func_sample();
}
