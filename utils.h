#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstddef>
#include <string>
#include <bitset>

#include "config.h"

namespace SIMPLE_DB {


	bool is_prime_number(int n);

	std::vector<int> produce_prime_number(int n) ;
		 
	//从bitset 中恢复每个字符的ascii码，bitset的低位是正真的地位，高位是真的高位，从bitset
	unsigned int trans_char(std::bitset<key_bitset_long>b, int st);	

	unsigned int trans_char_value(const std::bitset<value_bitset_long>& b, int st);


	std::bitset<key_bitset_long> trans_string_to_key(const std::string& Kst);



	std::string trans_key_to_string(std::bitset<key_bitset_long> kbt);



	std::bitset<value_bitset_long> trans_string_to_value(const std::string Kst);



	std::string trans_value_to_string(std::bitset<value_bitset_long> kbt);

	//抄来的代码，获取文件夹里边所有的文件
	bool get_all_files(std::string dir_in, std::vector<std::string> files);
}

