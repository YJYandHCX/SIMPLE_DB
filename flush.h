#pragma once 
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
#include<iostream>
#include<stdio.h>
#include<fstream>
#include <atomic>

#include "config.h"
#include "log_lib.h"
#include "memtable.h"
#include "bloomfilter.h"
#include "run_map.h"

/*这里这个文件是用于flush 数据，可能用一个两个线程池来做，*/

namespace SIMPLE_DB{
    
    //std::string load_memtable_from_sst_2(std::string file_name){
    class run_map;

    bool dump_file(std::string file_name,std::vector<Node>& data);

    std::vector<Node> load_memtable_from_sst_2(std::string file_name);


    void flush_33(std::shared_ptr<memtable> mem,bloomfilter* bf,int id,run_map* rm);

    //用于关闭数据库时候的写入最后内存中memtable的操作,用memtable
    void flush_4(memtable* mem,bloomfilter* bf,int id,run_map* rm);


    void flush_3(memtable* mem,bloomfilter* bf,int id,run_map* rm);


    void flush_2(memtable* mem,bloomfilter* bf,int id);


    void flush(memtable* mem,int id);

    std::vector<Node> load_memtable_from_sst(const std::string& file_name);


    std::vector<SIMPLE_DB::Node> load_sst(std::string file_name);
    
    void load_sst_2(std::string file_name,std::vector<Node>& res);
}
