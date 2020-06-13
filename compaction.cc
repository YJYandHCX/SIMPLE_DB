#include "compaction.h"

using namespace SIMPLE_DB;

void SIMPLE_DB::compaction(database *db){
        
        int nums = db->L0_file_num;
        
        std::vector<Node> ji;
    
        int ssss_t = (MEMTABLE_SIZE/sizeof(Node)* (db->L0_file_num));
        
    
        ji.resize(ssss_t);

        
        
        int ind = 0;
        
        

        for (int i  = 0;i<nums;i++){

            std::cout<<i<<std::endl;
            
            std::string s_p = std::to_string(i) + ".l0";

            std::ifstream ifs(s_p);
            
            std::string content((std::istreambuf_iterator<char>(ifs)) ,(std::istreambuf_iterator<char>()));
            

            int k = content.size()/(key_bitset_long + value_bitset_long);
            
            int offset = 0;
            
            for (int j = 0;j<k;j++){
                Node tmp;
            
                for (int qp =0; qp<key_bitset_long;qp++){
            
                    if(content[offset] == '1') tmp.key[qp] = 1;
                    
                    else tmp.key[qp] = 0;
                    
                    offset++;
                }

                for (int qp =0;qp<value_bitset_long;qp++){
                    if(content[offset] == '1') tmp.value[qp] = 1;
                    else tmp.value[qp] = 0;
                    offset++;
                }
                ji[ind] = tmp;
                ind++;
            }
        }
        
        for (int i = 0;i<ji.size();i++){

            for (int j = 0;j<i;j++){

                if (!SIMPLE_DB::cmp(ji[j+1],ji[j])) std::swap(ji[j+1],ji[j]);  

            }

        }



        int l_s = ji.size()*(key_bitset_long+value_bitset_long);

		std::string r(l_s,'0');
		int offset = 0;


		for (int n = 0;n<ji.size();n++){
			for (int i = 0;i<key_bitset_long;i++){

				if (ji[n].key[i] == 1) r[offset] = '1';
				offset++;
			}
				

			for (int i = 0;i<value_bitset_long;i++){
            
                if(ji[n].value[i] == 1) r[offset] = '1';
			
            		offset++;
			}

		}
        //std::cout<<"OKKKK"<<std::endl;
        
        std::string file_name = std::to_string(db->L1_file_num)+".l1";
        
        int fd = open(file_name.c_str(),O_WRONLY|O_CREAT);
        
		assert(fd>0);
        
		write(fd,r.c_str(),r.size());
        
		close(fd); 

        for (int i = 0;i<nums;i++){

            std::string s_p = std::to_string(i) + ".l0";
            remove(s_p.c_str());
        }
        
        
        while(db->locker.test_and_set(std::memory_order_acquire));
        
        db->L1_file_num++;
        
        db->bloomfilters[0]->reset();
        
        db->bloomfilters[0]->to_file("0.bf");
        
        db->L0_file_num = 0;

        for (auto x:ji){
            db->bloomfilters[1]->SetKey(x.key);
        }
        db->bloomfilters[1]->to_file("1.bf");
        
        delete db->run_maps[0];

        db->run_maps[0] = new run_map(0); 
        
		db->run_maps[1]->insert(db->L1_file_num-1,ji.front().key,ji.back().key);

        (db->locker).clear(std::memory_order_release);
        
        return;
}   
