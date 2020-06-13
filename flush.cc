#include "flush.h"

using namespace SIMPLE_DB;

    //std::string load_memtable_from_sst_2(std::string file_name){


bool SIMPLE_DB::dump_file(std::string file_name,std::vector<Node>& data){
        int l_s = data.size()*(key_bitset_long+value_bitset_long);

        std::string r(l_s,'0');

        int offset = 0; 
        

        for(int i = 0; i<data.size();i++){

            for (int j =0;j<key_bitset_long;j++){
            
                if(data[i].key[j] == 1) r[offset] = '1';
                
                offset++;
            }
            
            for (int j =0;j<value_bitset_long;j++){
            
                if (data[i].value[j] == 1) r[offset] = '1';
                offset++;
            }
        }

        int fd = open(file_name.c_str(),O_WRONLY|O_CREAT);
        
        write(fd,r.c_str(),r.size());
        
        close(fd);
        
        return true;
}
    
std::vector<Node> SIMPLE_DB::load_memtable_from_sst_2(std::string file_name){
        
        std::vector<Node> res(MEMTABLE_SIZE/(sizeof(Node)));

        int fd = 0;

        std::ifstream ifs(file_name);

        std::string content((std::istreambuf_iterator<char>(ifs)) ,(std::istreambuf_iterator<char>()));
        
        int offset = 0; 


        for(int i = 0; i<(MEMTABLE_SIZE/(sizeof(Node)));i++){

            for (int j =0;j<key_bitset_long;j++){

                if(content[offset] == '1') res[i].key[j] = 1;

                else res[i].key[j] = 0;

                offset++;

            }

            for (int j =0;j<value_bitset_long;j++){

                if(content[offset] == '1') res[i].value[j] = 1;

                else res[i].value[j] = 0;

                offset++;
            }
        }
        return res;
}

void SIMPLE_DB::flush_33(std::shared_ptr<memtable> mem,bloomfilter* bf,int id,run_map* rm){

        //std::ofstream ofile;

        int n = 10000;


        //std::vector<Node> r(MEMTABLE_SIZE/sizeof(Node));

        int fd = 0;

        std::string file_name;

        while(n--){

            file_name = std::to_string(id) + ".l0";

            fd = creat(file_name.c_str(),O_CREAT);

            if (fd<0) continue;

            else break;

        }



        std::string r = mem->seq_data_2();

        fd = open(file_name.c_str(),O_WRONLY);

        write(fd,r.c_str(),r.size());

        close(fd);

        for (auto x:mem->data){

            bf->SetKey(x.key);

        }

        bf->to_file("0.bf");

        rm->insert(id,mem->K_min,mem->K_max);

        //delete mem;

	}

    //用于关闭数据库时候的写入最后内存中memtable的操作,用memtable
void SIMPLE_DB::flush_4(memtable* mem,bloomfilter* bf,int id,run_map* rm){

        //std::ofstream ofile;

        int n = 10000;

        //std::vector<Node> r(MEMTABLE_SIZE/sizeof(Node));

        int fd = 0;

        std::string file_name;

        while(n--){

            file_name = std::to_string(id) + ".l0";

            fd = creat(file_name.c_str(),O_CREAT);

            if (fd<0) continue;

            else break;
        }


        mem->sort();
        
        std::string r = mem->seq_data_2();
        
        //std::cout<<r<<std::endl;
        fd = open(file_name.c_str(),O_WRONLY);

        write(fd,r.c_str(),r.size());

        close(fd);

		//放入布隆过滤器
        for (auto x:mem->data){

            bf->SetKey(x.key);

        }

        bf->to_file("0.bf");

        rm->insert(id,mem->K_min,mem->K_max);

        delete mem;
	}
void SIMPLE_DB::flush_3(memtable* mem,bloomfilter* bf,int id,run_map* rm){
        //std::ofstream ofile;
        int n = 10000;

        //std::vector<Node> r(MEMTABLE_SIZE/sizeof(Node));
        int fd = 0;

        std::string file_name;

        while(n--){

            file_name = std::to_string(id) + ".l0";

            fd = creat(file_name.c_str(),O_CREAT);

            if (fd<0) continue;

            else break;
        }



        std::string r = mem->seq_data_2();

        fd = open(file_name.c_str(),O_WRONLY);

        write(fd,r.c_str(),r.size());

        close(fd);

        for (auto x:mem->data){

            bf->SetKey(x.key);

        }

        bf->to_file("0.bf");

        rm->insert(id,mem->K_min,mem->K_max);

        delete mem;
}


void SIMPLE_DB::flush_2(memtable* mem,bloomfilter* bf,int id){

        //std::ofstream ofile;

        int n = 10000;

        //std::vector<Node> r(MEMTABLE_SIZE/sizeof(Node));
        int fd = 0;

        std::string file_name;

        while(n--){

            file_name = std::to_string(id) + ".l0";

            fd = creat(file_name.c_str(),O_CREAT);

            if (fd<0) continue;

            else break;
        }



        std::string r = mem->seq_data_2();
        fd = open(file_name.c_str(),O_WRONLY);
        write(fd,r.c_str(),r.size());
        close(fd);
        for (auto x:mem->data){
            bf->SetKey(x.key);
        }
        bf->to_file("0.bf");
        delete mem;
	}
void SIMPLE_DB::flush(memtable* mem,int id){
        //std::ofstream ofile;
        int n = 10000;

        int fd = 0;
        std::string file_name;
        while(n--){

            file_name = std::to_string(id) + ".l065";

            fd = creat(file_name.c_str(),O_CREAT);
            if (fd<0) continue;

            else break;
        }

        std::string r = mem->seq_data();

        fd = open(file_name.c_str(),O_WRONLY);


        write(fd,r.c_str(),r.size());

        close(fd);
        
		delete mem;
}

std::vector<Node> SIMPLE_DB::load_memtable_from_sst(const std::string& file_name){
        //std::ifstream ifile(file_name.c_str(), std::ios::binary);
        int fd = open(file_name.c_str(),O_RDONLY);
        //.std::vector<Node> res(MEMTABLE_SIZE/sizeof(Node));
        char res [(MEMTABLE_SIZE)/sizeof(Node)*(key_bitset_long+value_bitset_long)];


        read(fd,(char*)(&res),sizeof((MEMTABLE_SIZE)/sizeof(Node)*(key_bitset_long+value_bitset_long)));

        close(fd);
        std::vector<Node> r((MEMTABLE_SIZE)/sizeof(Node));

        //std::cout<<res<<std::endl;
        
        int offset = 0;
        for (int i = 0;i<(MEMTABLE_SIZE)/sizeof(Node);i++){
            for(int j = 0;j<key_bitset_long;j++){
                if (res[offset] == '1'){
                    r[i].key[j] = 1;
                }
                else r[i].key[j] = 0;
                offset++;
            }
            for (int j = 0;j<value_bitset_long;j++){
                if (res[offset] == '1'){
                    r[i].value[j] = 1;
                }
                else r[i].value[j] = 0;
                offset++;
            }
        }
        //ifile>>res;
        //ifile.read((char*)&res, sizeof(res));
        //ifile.close();
        return r;
}

std::vector<SIMPLE_DB::Node> SIMPLE_DB::load_sst(std::string file_name){
        
        

        std::string line;
        
        std::ifstream myfile (file_name,std::ios_base::in);

        getline (myfile,line);


        myfile.close();
        //std::cout<<line<<std::endl;
        //std::cout<<line.size()<<endl;
        int block_l = key_bitset_long + value_bitset_long;
        int num = line.size()/block_l;
        
        //std::cout<<line.size()<<std::endl;
        
        std::vector<Node> res(num);
        for (int i = 0;i<num;i++){
            //std::bitset<key_bitset_long> key;
            //std::bitset<value_bitset_long> value;
            

            for (int j = 0;j<key_bitset_long;j++){

                if (line[i*block_l + j ] == '1') res[i].key[j] = 1;
                else res[i].key[j] = 0;
            }

            for (int j = 0;j<value_bitset_long;j++){
                
                if (line[i*block_l+j+key_bitset_long] == '1') res[i].value[j] =1;
                
                else res[i].value[j] = 0;

            }

        }
        return res;
}
    
void SIMPLE_DB::load_sst_2(std::string file_name,std::vector<Node>& res){
        
        

        std::string line;
        
        std::ifstream myfile (file_name,std::ios_base::in);

        getline (myfile,line);


        myfile.close();
       
        int block_l = key_bitset_long + value_bitset_long;
        int num = line.size()/block_l;
        
        res.resize(num);
        //std::vector<Node> res(num);
        for (int i = 0;i<num;i+=1){
            std::bitset<key_bitset_long> key;
            std::bitset<value_bitset_long> value;
            

            for (int j = 0;j<key_bitset_long;j++){

                if (line[i*block_l + j ] == '1') res[i].key[j] = 1;
                else res[i].key[j] = 0;
            }

            for (int j = 0;j<value_bitset_long;j++){
                
                if (line[i*block_l+j+key_bitset_long] == '1') res[i].value[j] =1;
                
                else res[i].value[j] = 0;

            }

        }
        return;
}
