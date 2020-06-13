#include "threadpool.h"
using namespace SIMPLE_DB;

Threadpool::Threadpool(int n):is_runing(true),threads_num(n),locker(ATOMIC_FLAG_INIT){

    //locker = ATOMIC_FLAG_INIT;
    auto p = [&](){
            
        while(is_runing){
            
            while(locker.test_and_set(std::memory_order_acquire));
            

            if (task_list.empty()){
            
                locker.clear(std::memory_order_release);
                continue;
            }
                
            auto task = task_list.back();
            
            task_list.pop_back();
            
            locker.clear(std::memory_order_release);
                
            task();
            
        }
            //std::cout<<"I am done"<<std::endl;
    };

        //locker = std::ATOMIC_FLAG_INIT;
    for (int i = 0;i<threads_num;i++){
        threads.push_back(std::move(std::thread(p)));
            
            //把所有工作线程设置为分离的
        threads[i].detach();
    }
}
    

Threadpool::~Threadpool(){
        //std::unique_lock<std::mutex> locker(mu);

        //这一块代码的作用就是，让所有的任务都做完之后再退出
        
    while(1){
        //std::cout<<"NOODDDDDDDD"<<std::endl;
        
        while(locker.test_and_set(std::memory_order_acquire));
            
        //std::cout<<"OOOOOOOOOOOOOOOOOOOOO"<<std::endl;
        if (task_list.empty()){
        
            locker.clear(std::memory_order_release);
        
            break;
        }
        
        else{
        
            locker.clear(std::memory_order_release);
        
            continue;
        }

    }

        //std::cout<<"I am prepare"<<std::endl;
  
    is_runing = false;
    
        //std::cout<<"I am prepare"<<std::endl;
        
        //静静等待所有的进程都退出
    sleep(3);
}


bool Threadpool::push_task(std::function<void()> task){
    
        //放入所有的任务
    while(1){
    
        while(locker.test_and_set(std::memory_order_acquire));

        task_list.push_front(task);
            
        locker.clear(std::memory_order_release);    
            
        return true;
    }
};
