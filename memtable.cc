#include "memtable.h"

using namespace SIMPLE_DB;
/*
相当于一个索引，记录每个文件起始值和终止值

*/
memtable::memtable(int space,int dd):locker(ATOMIC_FLAG_INIT),is_freeze(false),id(dd) {
        
    node_size = sizeof(Node);
		
    size = 0;
		
    max_kv_num = space / node_size;
		
        //std::cout<<max_kv_num<<std::endl;
    data.reserve(max_kv_num);
}
	
memtable::memtable(const memtable& a){

        this->data = a.data;

        return;
};

Node* memtable::find(const std::bitset<key_bitset_long>& Key) {
	
    while(locker.test_and_set(std::memory_order_acquire));
	
    Node* r = nullptr;
		
	for (auto p = data.begin(); p != data.end(); p++) {

		if (p->key == Key) break;
	}
		
    locker.clear(std::memory_order_release);
		
    if (r == nullptr) return nullptr;
		
    return &(*r);
}

	
bool memtable::insert(Node a) {
	auto pre_p = find(a.key);

	while (locker.test_and_set(std::memory_order_acquire));

	++size;

		
	if (pre_p == nullptr) {
			
		if (size >= max_kv_num) {
			data.push_back(a);
				
                //std::cout << data.size() << std::endl;

			std::sort(data.begin(), data.end(),cmp);

			K_min = data[0].key;
				
            K_max = (data.back()).key;

			locker.clear(std::memory_order_release);
            
            is_freeze = true;
            return false;
		}

			
        else {
			
            data.push_back(a);
			
            locker.clear(std::memory_order_release);
			
            return true;
		}
	}

	else {
			//auto pre_p = find(a.key);
			
        pre_p->value = a.value;
			
        locker.clear(std::memory_order_release);
			
			
        if (size >= max_kv_num) return false;
			
         return true;
	}
}

	
bool memtable::remove(std::bitset<key_bitset_long> d_k) {
		
	Node* pre_p = find(d_k);
		
    while (locker.test_and_set(std::memory_order_acquire));
		
    if (pre_p == nullptr) {
			
        locker.clear(std::memory_order_release);
			
        return false;
	}
		
    else {

		//pre_p->delete_time = time(NULL);
			
		for (int i = 0;i<key_bitset_long;i++){
			pre_p->key[i] = 0;
        }
			
        this->locker.clear(std::memory_order_release);
			
        return true;
	}
}

bool SIMPLE_DB::cmp(Node a, Node b){
			//while (locker.test_and_set());
	
    for (int i = key_bitset_long-1; i>=0 ; i--) {
				
        if (a.key[i] > b.key[i]) return false;
				
        if (a.key[i] < b.key[i]) break;
	}
			//locker.clear();
			
    return true;
};
