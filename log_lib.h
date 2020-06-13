#pragma once
#include <map>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cassert>
#include <time.h>
#include <mutex>
#include <atomic>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <regex>

#include "config.h"

/*我这里的想法是，工作线程把数据考到缓冲区，然后一个线程专门检测有么有字符，如果有就写入，因为是日志比较重要，
用sync 同步写过去。所以效率比较，慢。用一个专门的线程来做，提升效率，也较少fd的线程安全问题。采用尾部追加的方式来做*/

namespace SIMPLE_DB{

    class LOG_SYS{
    
    public:

        //日志缓冲区
        char buffer[LOG_BUFFER_SIZE];
        


        LOG_SYS();
        
        ~LOG_SYS();

        bool append(std::string t);

        static void cycle(LOG_SYS* p){
            
            while(p->is_runing){


                sleep(1);

                //自旋
                while(p->locker.test_and_set(std::memory_order_acquire));
                

                //如果没有数据
                if (p->cur_offset == 0){

                    p->locker.clear(std::memory_order_release);

                    continue;
                }

                else{

                    write(p->log_fd,p->buffer,p->cur_offset+1);
                    
                    sync();

                    //传输完之后，清理一下缓冲区
                    memset(p->buffer,0,sizeof(buffer));
                    
                    //把偏置设置回去
                    p->cur_offset = 0;
                    
                    //释放锁
                    p->locker.clear(std::memory_order_release);
                }
            }
            close(p->log_fd);
            
            //std::cout<<"LOG system is closed"<<std::endl;
        }
    private:
        
        //现在用了多少的offeset

        int cur_offset;

        //日志文件的句柄；
        int log_fd;
        //简单自旋锁
        std::atomic_flag locker;                

        //结束的标记
        std::atomic_bool is_runing;
    
        std::string log_path;

    };
}
