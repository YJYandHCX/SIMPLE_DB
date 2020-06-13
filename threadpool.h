#pragma once
#include <thread>
#include <vector>
#include <deque>
#include <functional>
#include <atomic>
#include <unistd.h>

namespace SIMPLE_DB{
    
    class Threadpool{
    public:

        Threadpool() = delete;
        
        Threadpool(int n);

        ~Threadpool();

        bool push_task(std::function<void()> task); 
    private:

        std::atomic<bool> is_runing;

        std::atomic_int threads_num;

        std::deque<std::function<void(void)>> task_list;

        std::vector<std::thread> threads;

        std::atomic_flag locker;
    };
}
