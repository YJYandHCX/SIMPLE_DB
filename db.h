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

#include "utils.h"
#include "config.h"
#include "log_lib.h"
#include "memtable.h"
#include "flush.h"
#include "threadpool.h"
#include "bloomfilter.h"
#include "run_map.h"
#include "LRU_cache.h"

namespace SIMPLE_DB{

    class database{

    public:

        //这个用于compaction的回调函数
        
		std::function<void(database*)> compaction_call_back;

        //run_map 用于记录每层的每个文件的最小和最大的key
        
		std::vector<run_map*> run_maps;

        //红黑树来存所有的memtalble，根据


        //读缓存对象
        
		LRU_cache* reading_cache;
        
        //是用一个map存id和memtabls
		
        std::map<int,memtable*> memtables_list;

        //当前memtable，下一个 memable可以用的id号
        
		int cur_memtables_id;


        //日志类的实例
        
		LOG_SYS* log_instance;
        
        
        //日志后台数据的句柄
        
        std::thread log_thread;
        
        //用于flues 的线程池
        
        Threadpool* flush_threadspool;

       
        //同步的锁
       
     	std::atomic_flag locker;

        //该数据库是不是创建的，还是原来都存在的，如果不是创建的
        //要去读有多少L0，L1文件，创建日志，还要做恢复，实现持久性
        
		std::atomic_bool is_create;

        //整个数据库lsm-tree的纵沟层数
        //std::atomic_int level_num;
        
		int level_num;
        
		//L0 的文件数
        //std::atomic_int L0_file_num;
        
		int L0_file_num;
        
		//L1 的文件数
        //std::atomic_int L1_file_num;
        
		int L1_file_num;

        //std::atomic_int L2_file_num;
        int L2_file_num;
        //互斥锁用来做同步

        //std::mutex;


        //每一次布隆过滤器布隆过滤器
        std::vector<bloomfilter*> bloomfilters;
		
        //Put来做
		bool Put(std::string K,std::string V);

        //get
		std::string Get(std::string K);
        
        database();
        
		//删除 ，crud全了
        bool Delete(std::string K);
        
        ~database();
    };

}
