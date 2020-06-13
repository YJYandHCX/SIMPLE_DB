#pragma once 
#include <map>
#include <bitset>
#include "config.h"
#include "utils.h"
#include "flush.h"


/*
相当于一个索引，记录每个文件起始值和终止值
*/
namespace SIMPLE_DB{

    std::vector<SIMPLE_DB::Node> load_sst(std::string file_name);

    class run_map{
    public:
        
        run_map(int level);
        
        ~run_map() = default;
        
        int get_level() {return LEV;};

        bool insert(int id,std::bitset<key_bitset_long> vv, std::bitset<key_bitset_long> ww);

        std::vector<int> return_id(std::bitset<key_bitset_long> n_key);

        
        bool load(int num);

        bool cmp(std::bitset<key_bitset_long> a, std::bitset<key_bitset_long> b);

        void show(){
            for (auto p = st.begin();p!=st.end();p++){

                auto pp = ed.find(p->first);
                std::cout<<trans_key_to_string(p->second)<<"   "<<trans_key_to_string(pp->second)<<std::endl;
            }
        }
    private:
    
        int LEV;
    
        std::map<int,std::bitset<key_bitset_long>> st;
    
        std::map<int,std::bitset<key_bitset_long>> ed;
    
    };
}
