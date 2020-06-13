#include "LRU_cache.h"

using namespace SIMPLE_DB;
void LRU_cache::set(std::string a,std::string b){
        if (index.count(a)!=0) return;

        data.push_front({a,b});

        index.insert({a,data.begin()});

       size++;

        if (size>size_limit){

            auto p = data.back().first;

            if (index.count(p) != 0){

                auto q = index.find(p);

                index.erase(q);
            }   

            data.pop_back();

            size--;
        }
        return;
}
    
    //如果没有返回nullptr 如果有就返回地址
std::string* LRU_cache::get(std::string& K){
    if (index.count(K) == 0) return nullptr;
        
    auto p = index.find(K)->second;

    auto pir = *p;
        
    data.erase(p);

    data.push_front(pir);

    return &(data.begin()->second);
}
    
bool LRU_cache::remove(std::string& K){
    if (index.count(K) == 0) return false;

    auto p = index.find(K);
    auto q = p->second;

    index.erase(p);

    data.erase(q);

    size--;

    return true;
}
