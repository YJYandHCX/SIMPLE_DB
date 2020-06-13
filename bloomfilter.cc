#include "bloomfilter.h"

using namespace SIMPLE_DB;

bloomfilter::bloomfilter(int m, int n) {


	//根据论文讲，m_k 的比较好的取值就是这个
	m_k = static_cast<int>(ceil((m / n)*log(2)));

	seeds = produce_prime_number(m_k+1);
	for (int i = 0; i < (BLOOMFILTER_LENGTH); i++) bit[i] = 0;
} 
	
hashcode_t bloomfilter::RSHash_bitset(const std::bitset<key_bitset_long>& k, int seed) {
		 
	hashcode_t a = 63689;

	hashcode_t hash = 0;


	for (int i = 0; i < key_bitset_long; i += 8) {

		hash = hash *a + trans_char(k, i);
		a *= seed;
	}

	return (static_cast<int>(hash) & 0x7FFFFFFF);
}

hashcode_t bloomfilter::RSHash( const char *str,int seed) {
	//hashcode_t bloomfilter::RSHash(const char *str, int seed) {

	hashcode_t a = 63689;

	hashcode_t hash = 0;


	while (*str) {

		hash = hash * a + (*str++); //++ 和*是一样的优先级
		a *= seed;
	}

	return (static_cast<int>(hash) & 0x7FFFFFFF);
}


void bloomfilter::SetKey(const char* str) {
	std::vector<int> ji(m_k + 1, 0);

	for (int i = 0; i <= m_k; ++i) {
		ji[i] = static_cast<int>(RSHash(str, seeds[i])) % (BLOOMFILTER_LENGTH);
	}

	for (int j = 0; j < m_k; j++) {
		bit[ji[j]] = 1;
	}
	return;
};

void bloomfilter::SetKey(const std::bitset<key_bitset_long>& a ) {
	
    std::vector<int> ji(m_k + 1, 0);

		for (int i = 0; i <= m_k; ++i) {

			//哈希值求出来
			ji[i] = static_cast<int>(RSHash_bitset(a, seeds[i])) % (BLOOMFILTER_LENGTH);
		}

		for (int j = 0; j < m_k; j++){
		
			//如果超过限制，打印出来
			if (ji[j] >= BLOOMFILTER_LENGTH) std::cout << "OUT" << std::endl;
	
			bit[ji[j]] = 1;

		}
			
	return;
}



bool bloomfilter::VaryExit(const std::bitset<key_bitset_long>& key) {
		
	std::vector<int> ji(m_k + 1, 0);

	for (int i = 0; i <= m_k; ++i) {

		ji[i] = static_cast<int>(RSHash_bitset(key, seeds[i])) % (BLOOMFILTER_LENGTH);
	
	}

	for (int j = 0; j < m_k; j++) {
		
		if (!bit[ji[j]]) return false;
	
	}

	return true;

}


void bloomfilter::to_file(std::string save_path){
        
	std::string t_r((BLOOMFILTER_LENGTH),'0');

    for (int i = 0;i<(BLOOMFILTER_LENGTH);i++){

        if (bit[i] == 1) t_r[i] = '1';
        
	}

    int fd = open(save_path.c_str(),O_WRONLY|O_CREAT);

    write(fd,t_r.c_str(),t_r.size());

	close(fd);
        
	return;

}

void bloomfilter::load(std::string save_path){
        
	int fd = 0;
        
	std::ifstream ifs(save_path);
        
	std::string content((std::istreambuf_iterator<char>(ifs)) ,(std::istreambuf_iterator<char>()));
        
	for (int i = 0;i<(BLOOMFILTER_LENGTH);i++){
            
		if (content[i] == '1') bit[i] = 1;
            
		else bit[i] = 0;
            //if (bit[i] == 1) t_r[i] = '1';
    }
    
}

void bloomfilter::reset(){
		
	for(int i = 0;i<BLOOMFILTER_LENGTH;i++) bit[i] = 0;

};
