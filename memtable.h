#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <functional>
#include <atomic>
#include <bitset>
#include <algorithm>
#include <unistd.h>

#include "config.h"

#include "utils.h"



namespace SIMPLE_DB {

	/*
	std::vector<int> produce_prime_number(int n) ;	 
	//从bitset 中恢复每个字符的ascii码，bitset的低位是正真的地位，高位是真的高位，从bitset
	unsigned int trans_char(std::bitset<key_bitset_long>b, int st);	
	unsigned int trans_char_value(const std::bitset<value_bitset_long>& b, int st);
	*/

	//抄来的代码，获取文件夹里边所有的文件
	//bool get_all_files(const std::string& dir_in, std::vector<std::string>& files);
	
	//k,v结构体 ，有简单的时间戳，在memtable中，和L0时候，
	//被删除只是被打上简单的

	
	struct Node {

		std::bitset<key_bitset_long> key;
		std::bitset<value_bitset_long> value;


		//这就是简单的时间戳
		time_t create_time;
		time_t delete_time;

        Node() = default;
		Node(std::bitset<key_bitset_long> a, std::bitset<value_bitset_long> b) :key(a), value(b) {

			//这里是提交了之后，才会设置，这保证隔离性，不会导致脏读
			create_time = 0;

			//内存里边和L0 不会真的删除，只是在这一行记录删除时间
			delete_time = 0;
		};
	};

	bool cmp(Node a, Node b);
	//std::function<bool(Node a,Node b)> cmp;
	class memtable {
	public:
		
		
		std::atomic_bool is_freeze;
		

        memtable(const memtable& a);
		memtable(int space=512,int id=100);

        ~memtable(){
            while(locker.test_and_set(std::memory_order_acquire));
        }
		//这里没有指针所有直接默认删除就好


        int id;
		//~memtable() = default;
		Node* find(const std::bitset<key_bitset_long>& Key);

		//插入,插入返回true,如果满了返回false
		bool insert(Node a);

		//用于按键值寻找
		

		//用于按照键值删除
		bool remove(std::bitset<key_bitset_long> d_k);

		//原子锁，保证多并发，乐观自旋锁降低开销
		std::atomic_flag locker;
	
		std::string seq_data_2(){
			int l_s = data.size()*(key_bitset_long+value_bitset_long);

			std::string r(l_s,'0');

			int offset = 0;
			
			for (int n = 0;n<data.size();n++){

				for (int i = 0;i<key_bitset_long;i++){

					if (data[n].key[i] == 1) r[offset] = '1';

					offset++;
				}
				

				for (int i = 0;i<value_bitset_long;i++){

                    if(data[n].value[i] == 1) r[offset] = '1';

					offset++;
				}

			}
			return r;
		}

        std::string seq_data(){
            
            int l_s = max_kv_num*(key_bitset_long+value_bitset_long);

			int offset = 0;

            std::string r(l_s,'0');
            

            for (int n = 0;n<max_kv_num;n++){
            
                for (int i = 0;i<key_bitset_long;i++){
            
                    //if (data[n].key[i] == 1) r[n*(key_bitset_long+value_bitset_long)+i] = '1';

					if (data[n].key[i] == 1) r[offset] = '1';

					offset++;
                }
                for (int i = 0;i<value_bitset_long;i++){

                    if(data[n].value[i] == 1) r[offset] = '1';

					offset++;
				}
            }
			            
			return r;
        };

		int get_max_size() {
			return max_kv_num;
		}
		int get_size() {
			return size;
		}

        std::vector<Node> get_data(){return data;};
		
		
		//刷写入硬盘时候记录
		std::bitset<key_bitset_long> K_min;

		//同上
		std::bitset<key_bitset_long> K_max;
		
		void sort(){
			std::sort(data.begin(),data.end(),cmp);
			K_min = data[0].key;
			K_max = data.back().key;
		};
		
		void show() {


			for (Node x : data) {

                auto kk = trans_key_to_string(x.key);

                auto pp = trans_value_to_string(x.value);
                
				std::cout<<kk<<"  "<<pp<<std::endl;
			}
		}
		std::vector<Node> data;
	private:

		

		//每个kv的大小
		int node_size;

		//最大储存的block，在达到的时候，刷写入硬盘
		int max_kv_num;

		

		int size;

	};
}
