#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
#include "ThreadPool.h"
int func(int j){
    cout<<j;
    return 1;
}
int main()
{
    
    ThreadPool pool(4);
    std::vector< future<int> > results;
    pool.enqueue(func,4);
/*
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue(func,4)*[i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            }*/
      /*  );
    }
    */
    //for(auto && result: results)
      //  std::cout << result.get() << ' ';
    
    return 0;
}
