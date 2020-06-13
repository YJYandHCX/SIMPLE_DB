#include "utils.h"

using namespace SIMPLE_DB;
bool SIMPLE_DB::is_prime_number(int n) {

	int ban = static_cast<int>(sqrt(n)) + 1;

	for (int i = 2; i <= ban; i++) {
		if (n%i == 0) return false;
	}
	
    return true;
}

std::vector<int> SIMPLE_DB::produce_prime_number(int n) {
	
    std::vector<int> r{5,7,11,13,31,37,61};
	
    int now = 67;
		
	while ( int(r.size()) < int(n)) {

		if (SIMPLE_DB::is_prime_number(now)) r.push_back(now);
			
		now++;
	}
		
	return r;
}
		 
//从bitset 中恢复每个字符的ascii码，bitset的低位是正真的地位，高位是真的高位，从bitset
unsigned int SIMPLE_DB::trans_char(std::bitset<key_bitset_long>b, int st) {

	unsigned int r = 0;
	
    for (int i = st; i < st + 8; i++) r += (b[i] << (i - st));
	
    return r;
}


unsigned int SIMPLE_DB::trans_char_value(const std::bitset<value_bitset_long>& b, int st) {
	
    unsigned int r = 0;
	
    for (int i = st; i < st + 8; i++) r += (b[i] << (i - st));

	return r;
}


std::bitset<key_bitset_long>SIMPLE_DB:: trans_string_to_key(const std::string& Kst) {
	if (Kst.size() > (key_bitset_long / 8)) {
		throw("Length of string out of limitation");
	}

	std::bitset<key_bitset_long> res;
	
    for (int i = 0; int(i) < int(Kst.size()); i++) {
			
        int asc = int(Kst[i]);

		std::bitset<8> tmp(asc);
		for (int j = 0; j < 8; j++) {
				res[i * 8 + j] = tmp[j];
		}
	}

	return res;
}



std::string SIMPLE_DB::trans_key_to_string(std::bitset<key_bitset_long> kbt) {
	
    std::string res;
		
    for (int i = 0; i < key_bitset_long;i+=8) {
			int sc = SIMPLE_DB::trans_char(kbt,i);

			if (sc == 0) return res;
			res += char(sc);
	}

	return res;
}



std::bitset<value_bitset_long> SIMPLE_DB::trans_string_to_value(const std::string Kst) {
		
    if (Kst.size() > (value_bitset_long / 8)) {
		throw("Length of string out of limitation");
	}

	std::bitset<value_bitset_long> res;

	for (int i = 0;int(i) < int(Kst.size()); i++) {
		
        int asc = int(Kst[i]);

		std::bitset<8> tmp(asc);
		
        for (int j = 0; j < 8; j++) {
				res[i * 8 + j] = tmp[j];
			}
	}

	return res;
}



std::string SIMPLE_DB::trans_value_to_string(std::bitset<value_bitset_long> kbt) {
	std::string res;
	
    for (int i = 0; i < value_bitset_long; i += 8) {
			int sc = SIMPLE_DB::trans_char_value(kbt, i);

			if (sc == 0) return res;
			res += char(sc);
	}

	return res;
}

	//抄来的代码，获取文件夹里边所有的文件
bool SIMPLE_DB::get_all_files(std::string dir_in, std::vector<std::string> files) {
    if (dir_in.empty()) {
        return false;
    }
    	
    struct stat s;

    stat(dir_in.c_str(), &s);
    
    if (!S_ISDIR(s.st_mode)) {
        return false;
    }
    
    DIR* open_dir = opendir(dir_in.c_str());
    
    if (NULL == open_dir) {
        std::exit(EXIT_FAILURE);
    }
    	
    dirent* p = nullptr;
    
    while( (p = readdir(open_dir)) != nullptr) {
    
        struct stat st;
        
        if (p->d_name[0] != '.') {
            	//因为是使用devC++ 获取windows下的文件，所以使用了 "\" ,linux下要换成"/"
            	//std::string name = dir_in + std::string("\\") + std::string(p->d_name);
            std::string name = dir_in + std::string("") + std::string(p->d_name);
            
            stat(name.c_str(), &st);
            
            if (S_ISDIR(st.st_mode)) {
                	
                get_all_files(name, files);
            }
            
            else if (S_ISREG(st.st_mode)) {
                
                files.push_back(name);
            
            }
        }
    }
    	
    closedir(open_dir);
    
    return true;
}
