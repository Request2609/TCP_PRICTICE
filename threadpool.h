#pragma once
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
using namespace std;

typedef struct {
    
    void *(*func)(void*);   
    void * arg;
}thread_task_t;

typedef struct threadpool_t{
    
    pthread_mutex_t lock ;//用于锁住本结构体中的数据成员
    pthread_mutex_t  thread_conuter;//记录忙状态线程个数的锁
    pthread_cond_t queue_not_full;//当任务队列满时,添加任务的队列阻塞,等待此条件变量
    //当队列不为空时,通知等待任务的线程
    pthread_cond_t queue_not_empty;
    
    //存放线程池中每个线程的tid,数组
    pthread_t*threads;   
    //存放管理线程
    pthread_t adjust_tid;
    //任务队列
    thread_task_t *task_queue;

    //线程池最小线程数
    int min_thr_num;
    //线程池最大线程数
    int max_thr_num;
    //当前存活的线程数量
    int live_thr_num;
    //忙碌的线程数量
    int busy_thr_num ;
    //要销毁的线程数量
    int wait_exit_thr_num;

    //队头下标
    int queue_front;
    //队尾下标
    int queue_rear ;
    //队列中实际的任务数
    int queue_size ;
    //队列中可容纳的任务数上限
    int queue_max_size ;

    //标志位,线程池使用状态
    int shutdown ;

}threadpool_t;
threadpool_t *threadpool_create(int min_thr_num ,int max_thr_num,int queue_max_size);

threadpool_t *threadpool_create(int min_thr_num ,int max_thr_num,int queue_max_size){
    int i ;
    threadpool_t *pool=NULL;
    do{
        
        if((pool = (threadpool_t*)malloc(sizeof(threadpool_t)))==NULL){
                cout<<"malloc failed" <<endl;    
           }
        pool->min_thr_num =min_thr_num ;
        pool->max_thr_num =max_thr_num ;
        pool->busy_thr_num =0 ;
        pool->live_thr_num = min_thr_num ;
        pool->queue_size = 0 ;
        pool->queue_max_size = queue_max_size ;
        pool->queue_front = 0 ;
        pool->queue_rear = 0 ;
        pool->shutdown=false ;
        
        //根据最大线程上限数给线程开辟空间并清0
        pool->threads=(pthread_t*)malloc(sizeof(pthread_t)*max_thr_num);
        if(pool->threads==NULL){
            cout<<"malloc failed"<<endl;
            break;
        }
        memset(pool->threads,0,sizeof(pthread_t)*max_thr_num);
        
        //初始化任务队列
        pool->task_queue = (thread_task_t*)malloc(sizeof(thread_task_t)*queue_max_size);
        if(pool->task_queue==NULL){
            cout<<"malloc failed"<<endl;
            break;
        }
        //初始化互斥锁,条件变量
        
        if(pthread_mutex_init(&(pool->lock),NULL)!=0
           ||pthread_mutex_init(&(pool->thread_conuter),NULL)!=0
           ||pthread_cond_init(&(pool->queue_not_empty),NULL)!=0||pthread_cond_init(&(pool->queue_not_full),NULL)!=0){
            cout<<"init lock or cond fail"<<endl;
            break;
        }

        //启动min_thr_num个工作线程
        for(int i =0 ;i<min_thr_num;i++){
            pthread_create(&pool->threads[i],NULL,threadpool_thread,(void*pool));
            cout<<"start thread"<<"   "<<(unsigned int)pool->threads[i]<<endl;
        }
        pthread_create(&(pool->adjust_tid),NULL,adjust_thread,(void*)pool);
        return pool;
    }while(0);

    threadpool_free(pool);
    return NULL;
}

//向线程池中添加任务

int threadpool_add(threadpool_t*pool,void*(*func)(void*arg),void*arg){
    
    pthread_mutex_lock(&(pool->lock));
    
    while(1);
}

void *threadpool_thread(void*threadpool){

    
}
