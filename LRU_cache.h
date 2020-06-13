#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
//#include <pair>


//这个地方主要用于读取数据的缓存

//想法是实现LRU缓存，通过一个list来存数据，然后搞
//然偶读一次就把新的数据放在队头，插入也放在对头，然后，在实现
//超过size的限制之后，就删除
namespace SIMPLE_DB{
    
    class LRU_cache{
    public:

        LRU_cache(int max_size):size_limit(max_size),size(0){


        }
        int get_size(){return size;}

        void set(std::string a,std::string b);

        std::string* get(std::string& K);
  
  bool remove(std::string& K);
    private:

        int size_limit;

        int size;

        std::unordered_map<std::string,std::list<std::pair<std::string,std::string>>::iterator> index;

        std::list<std::pair<std::string,std::string>> data;    
    };
}
