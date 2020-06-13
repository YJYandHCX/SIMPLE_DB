#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <bitset>
#include <fstream>
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
#include "utils.h"

/* 布隆过滤器，用于查询的时候，在不在
这一个levle中，
采用rash 这个字符串哈希函数，
*/
namespace SIMPLE_DB {

	//rhash返回的都是正整数
	using hashcode_t = unsigned int;
	
	class bloomfilter {
	public:
		
		//素数表。用作多个seed，用rhash 一个函数达到效果
		std::vector<int> seeds;


		//
		bloomfilter(int m, int n);
	
		hashcode_t RSHash(const char *str,int seed);
		
		//哈希函数
		hashcode_t RSHash_bitset(const std::bitset<key_bitset_long>& k, int seed);
		
		void SetKey(const char* str);
		
		void SetKey(const std::bitset<key_bitset_long>& a);

		bool VaryExit(const std::bitset<key_bitset_long>& key);
				
		std::bitset<BLOOMFILTER_LENGTH> bit;

        void to_file(std::string save_path);

        void load(std::string save_path);


		void reset();

	private:

		//这个m_k是不同哈希函数的个数
	
	int m_k;
	
	};
	
}
