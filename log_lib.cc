#include "log_lib.h"

using namespace SIMPLE_DB;

LOG_SYS::LOG_SYS():cur_offset(0),is_runing(true),log_path(LOG_PATH),locker(ATOMIC_FLAG_INIT){
        //尝试打开，可能不存在
      
    log_fd = open(log_path.c_str(),O_APPEND|O_WRONLY);

        //若不存在
    if (log_fd<0){

        log_fd = creat(log_path.c_str(),O_CREAT);
            
            //追加读写的方式打开
        log_fd = open(log_path.c_str(),O_APPEND|O_WRONLY);
            
            
        assert(log_fd);

            //记录创建日志和数据库的时间
        char create_notice[] = "Create_the_SIMPLE_DB_at ";
            
        auto ti = std::to_string(time(NULL)) + '\n'; 
            
        ssize_t sta = write(log_fd,create_notice,sizeof(create_notice));
            
        assert(sta>=0);

        sta = write(log_fd,ti.c_str(),sizeof(ti.c_str()));
    }
        
    else{

            //记录打开数据日志的时间
        char t[] = "\nopen the SIMPLE DB at ";

        auto ti = std::to_string(time(NULL)) + '\n'; 
            
        ssize_t sta = write(log_fd,t,sizeof(t));
            
        assert(sta>=0);

        sta = write(log_fd,ti.c_str(),sizeof(ti.c_str()));
            
        assert(sta>=0);
    }
    
    memset(buffer,0,sizeof(buffer));
}

bool LOG_SYS::append(std::string t){
                    
    while(locker.test_and_set(std::memory_order_acquire));

        //如果缓冲区满了就返回false
    if(cur_offset+t.size() >= (LOG_BUFFER_SIZE)) {
        
        locker.clear(std::memory_order_release);
        
        return false;
    }
        

        //把日志拷贝到缓冲区里边
    for (int i = 0;i<t.size();i++){
        
        buffer[cur_offset] = t[i];
        
        cur_offset++;
        
    }
        

    locker.clear(std::memory_order_release);
        
    return true;
};
    //最好记得显示调用一下
LOG_SYS::~LOG_SYS(){
        
    //表示程序要结束，让后台日志同步线程结束
    is_runing = false;
        
        //等两秒
    sleep(2);
}
