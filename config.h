#pragma once
#include <iostream>

#define key_bitset_long 80
#define value_bitset_long 320


#define BLOOMFILTER_LENGTH 1<<22

#define LOG_BUFFER_SIZE 1024*1024*3

#define LOG_PATH "./RIZHI.log"

#define MEMTABLE_NUM 1

#define MEMTABLE_SIZE 1024*1024

#define DB_PATH "./"
//暂定数据库lsm tree的层数是3
#define LEVEL_NUM 3

//这里只能规定l1和l0的上限文件数目，l2是无限大没办法保证
#define L0_MAX_FILE_NUM 200

#define L1_MAX_FILE_NUM 100000

#define FLUSH_THREAD_NUM 4

#define MAX_KV_NUM_IN_CACHE 100000
