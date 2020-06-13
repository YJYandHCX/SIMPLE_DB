#include "run_map.h"

using namespace SIMPLE_DB;

run_map::run_map(int level){LEV = level;};
    
bool run_map::insert(int id, std::bitset<key_bitset_long> vv,std::bitset<key_bitset_long> ww){
        if (st.count(id) != 0) return false;
        st.insert({id,vv});
        ed.insert({id,ww});
        return true;
}

    //如果存在返回id，如果找不到返回-1
bool run_map::cmp(std::bitset<key_bitset_long> a , std::bitset<key_bitset_long> b){
	for (int i = key_bitset_long-1; i>=0 ; i--) {
				
        if (a[i]> b[i]) return false;
				
        if (a[i] < b[i]) break;
	}
			//locker.clear();
			
    return true;
};


std::vector<int> run_map::return_id(std::bitset<key_bitset_long> n_key){
    int n = 0;
        
    std::vector<int> res;
        
    while(1){
            
        if (st.count(n) == 0) break;
        
        auto oo = st.find(n)->second;
        
        auto pp = ed.find(n)->second;
            
        if (oo == n_key || pp == n_key){
            
            res.push_back(n);
        
        }

        if (cmp(oo,n_key) == false && cmp(pp,n_key) == true){
               
            res.push_back(n);
                
        }
            
        n++;
    }
    
    return res;
}
    
bool run_map::load(int num){

    for (int i = 0;i<num;i++){
        
        std::string s_p = std::to_string(i) + ".l" + std::to_string(LEV);
            
        auto t = load_sst(s_p);
            
        st.insert({i,t[0].key});
            
        ed.insert({i,t.back().key});
        
    }

    return true;
}
