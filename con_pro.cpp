#include <iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>

using namespace std;

//任务队列
queue<int>products ;
mutex m ;
condition_variable cond ;
bool notify = false ;
bool done = false ;

void producer() ;
void consumer(){
    
    while(!done){
        
        //上锁保护共享资源,unique_lock一次实现上锁和解锁
        unique_lock<mutex>lk(m);
        //等待生产者者通知有资源
        while(!notify){
            
            cond.wait(lk);
        }

        //要是队列不为空的话
        while(!products.empty()){

            cout<<"consumer..."<<products.front()<<endl;
            products.pop();
            //通知生产者仓库容量不足,生产产品
            notify = false ;
            cond.notify_one();
        }
    }
}

void producer(){
    
    int i ;
    for(i=0;i<10;i++){

        this_thread::sleep_for(chrono::seconds(1));
        unique_lock<mutex>lk(m);
        cout<<"producer..."<<i<<endl;
        //如果仓库中有产品,就等待消费者消费完后在生产
        while(notify||!products.empty()){
            cond.wait(lk);
        }
        //当前仓库里面没有东西了,就将产品装入仓库
        products.push(i);
        //设置有产品的通知
        notify = true ;
        //通知消费者可以取产品了
        cond.notify_one();
        
    }   
    

    //通知消费者端不生产了
    done = true ;
    cond.notify_one();
}   

int main()
{

    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

